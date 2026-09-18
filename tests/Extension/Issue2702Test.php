<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use ArrayAccess;
use PHPUnit\Framework\TestCase;
use ReturnTypeWillChange;
use Stub\Issue2702;
use Throwable;
use stdClass;

/**
 * `unset this->prop[offset]` against what PHP does with the same statement.
 *
 * The literal offset was stamped into a temp borrowed from a read-only
 * property fetch, so the zend_string it owned leaked; the property branch also
 * fell through into the generic array-access branch, so the unset ran twice
 * and an ArrayAccess property received two offsetUnset() calls; and the kernel
 * returned quietly for containers PHP reports an error for.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2702
 */
final class Issue2702Test extends TestCase
{
    private Issue2702 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2702();
    }

    /**
     * Asserted before every probe below: a flat reading from an instrument
     * that cannot see retention is indistinguishable from a pass.
     */
    public function testTheLeakProbeCanMeasureGrowth(): void
    {
        $this->subject->growthProbe(200);

        $this->assertGreaterThan(
            $this->subject->growthProbe(1000),
            $this->subject->growthProbe(50000),
            'The probe cannot measure retained memory, so the leak tests below are meaningless.'
        );
    }

    /**
     * The defect itself: one zend_string per call, nothing releasing it.
     *
     * @depends testTheLeakProbeCanMeasureGrowth
     */
    public function testALiteralStringOffsetDoesNotLeak(): void
    {
        $this->subject->literalProbe(100);

        $this->assertSame(
            $this->subject->literalProbe(1000),
            $this->subject->literalProbe(50000),
            'Retained memory grows with the iteration count, so the literal offset leaks.'
        );
    }

    /**
     * The control that was already flat: ZVAL_LONG allocates nothing, which is
     * why only string offsets ever leaked.
     *
     * @depends testTheLeakProbeCanMeasureGrowth
     */
    public function testALiteralLongOffsetDoesNotLeak(): void
    {
        $this->subject->longLiteralProbe(100);

        $this->assertSame(
            $this->subject->longLiteralProbe(1000),
            $this->subject->longLiteralProbe(50000)
        );
    }

    /**
     * The same literal against a local array, which never went through the
     * property path at all.
     *
     * @depends testTheLeakProbeCanMeasureGrowth
     */
    public function testALiteralOffsetOnALocalArrayDoesNotLeak(): void
    {
        $this->subject->localArrayProbe(100);

        $this->assertSame(
            $this->subject->localArrayProbe(1000),
            $this->subject->localArrayProbe(50000)
        );
    }

    /**
     * The double emission, made observable: the second unset re-read the
     * property and unset again, which on an ArrayAccess container is a second
     * userland call, not a no-op.
     *
     * @dataProvider offsetForms
     */
    public function testAnArrayAccessPropertyReceivesOneOffsetUnset(string $method, array $arguments): void
    {
        $bag = new Issue2702Bag();

        $this->subject->container = $bag;
        $this->subject->$method(...$arguments);

        $this->assertSame(1, $bag->unsetCalls, 'offsetUnset() must be called once, as PHP calls it.');
        $this->assertSame(['j' => 2], $bag->items);
    }

    public static function offsetForms(): array
    {
        return [
            'string literal' => ['unsetContainerLiteral', []],
            'var offset'     => ['unsetContainerByVar', ['k']],
        ];
    }

    /**
     * Every container type, asserted against PHP running the same statement
     * rather than against hardcoded text. PHP 8.0 reports nothing for a scalar
     * container and PHP 8.1 and later raise an Error, so a literal expectation
     * would have to encode the version; this cannot.
     *
     * @dataProvider containers
     */
    public function testAContainerBehavesAsItDoesInPhp(callable $make): void
    {
        $reference = new stdClass();
        $reference->container = $make();

        $expected = $this->outcomeOf(static function () use ($reference): void {
            unset($reference->container['k']);
        });

        $this->subject->container = $make();

        $actual = $this->outcomeOf(function (): void {
            $this->subject->unsetContainerLiteral();
        });

        $this->assertSame($expected, $actual);
        $this->assertEquals($reference->container, $this->subject->container);
    }

    /**
     * @dataProvider containers
     */
    public function testALongOffsetOnAContainerBehavesAsItDoesInPhp(callable $make): void
    {
        $reference = new stdClass();
        $reference->container = $make();

        $expected = $this->outcomeOf(static function () use ($reference): void {
            unset($reference->container[3]);
        });

        $this->subject->container = $make();

        $actual = $this->outcomeOf(function (): void {
            $this->subject->unsetContainerLongLiteral();
        });

        $this->assertSame($expected, $actual);
        $this->assertEquals($reference->container, $this->subject->container);
    }

    /**
     * The same container table against a local, which reaches the kernel
     * helpers directly. Nearly every `unset` in the wild has this shape, so
     * it is the widest reach of reporting what PHP reports.
     *
     * @dataProvider localOffsets
     */
    public function testALocalContainerBehavesAsItDoesInPhp(string $method, array $arguments, callable $php): void
    {
        foreach (self::containers() as $label => [$make]) {
            $expected = $this->outcomeOf(static function () use ($php, $make, &$phpResult): void {
                $phpResult = $php($make());
            });

            $actual = $this->outcomeOf(function () use ($method, $arguments, $make, &$zephirResult): void {
                $zephirResult = $this->subject->$method($make(), ...$arguments);
            });

            $this->assertSame($expected, $actual, "Container: $label");
            $this->assertEquals($phpResult, $zephirResult, "Container: $label");
        }
    }

    public static function localOffsets(): array
    {
        return [
            'string literal' => ['unsetLocalLiteral', [], static function ($c) { unset($c['k']); return $c; }],
            'long literal'   => ['unsetLocalLongLiteral', [], static function ($c) { unset($c[3]); return $c; }],
            'var offset'     => ['unsetLocalByVar', ['k'], static function ($c) { unset($c['k']); return $c; }],
        ];
    }

    public static function containers(): array
    {
        return [
            'array'           => [static fn () => ['k' => 1, 'j' => 2, 3 => 'three']],
            'array access'    => [static fn () => new Issue2702Bag()],
            'plain object'    => [static fn () => new stdClass()],
            'string'          => [static fn () => 'hello'],
            'int'             => [static fn () => 5],
            'float'           => [static fn () => 1.5],
            'true'            => [static fn () => true],
            'false'           => [static fn () => false],
            'null'            => [static fn () => null],
        ];
    }

    /**
     * A property holding a reference reached the kernel as IS_REFERENCE, which
     * matched no branch, so the unset silently did nothing. PHP follows the
     * reference first.
     */
    public function testAnUnsetReachesThroughAReference(): void
    {
        $shared      = &$this->subject->shared;
        $expected    = ['j' => 2];

        $this->subject->removeFromShared();

        $this->assertSame($expected, $shared);
        $this->assertSame($expected, $this->subject->shared);
    }

    /**
     * @dataProvider offsetTypes
     */
    public function testTheKeyIsRemovedAndItsSiblingsKept(string $method, array $arguments, array $expected): void
    {
        $this->subject->$method(...$arguments);

        $this->assertSame($expected, $this->subject->getData());
    }

    public static function offsetTypes(): array
    {
        return [
            'string literal' => ['removeLiteral', [], ['key_b' => 'b', 3 => 'three']],
            'long literal'   => ['removeLongLiteral', [], ['key_a' => 'a', 'key_b' => 'b']],
            'var'            => ['removeByVar', ['key_b'], ['key_a' => 'a', 3 => 'three']],
            'native string'  => ['removeByNativeString', ['key_b'], ['key_a' => 'a', 3 => 'three']],
            'native int'     => ['removeByNativeInt', [3], ['key_a' => 'a', 'key_b' => 'b']],
        ];
    }

    /**
     * The property's persistent default must not be reachable from the copy
     * taken before the unset, which is only true if the unset separated.
     */
    public function testTheUnsetSeparatesFromTheSharedDefault(): void
    {
        $this->assertSame(
            [
                ['a' => 1, 'b' => 2, 3 => 'three'],
                ['b' => 2, 3 => 'three'],
            ],
            $this->subject->snapshotAroundLiteralUnset()
        );

        $this->assertSame(
            ['a' => 1, 'b' => 2, 3 => 'three'],
            (new Issue2702())->getRemovable(),
            'A fresh instance must still see the untouched default.'
        );
    }

    /**
     * Runs a statement and reports everything PHP would let an observer see:
     * the error it threw, and the diagnostics it raised.
     *
     * @return array{error: string|null, diagnostics: array<int, string>}
     */
    private function outcomeOf(callable $statement): array
    {
        $diagnostics = [];

        set_error_handler(static function (int $number, string $message) use (&$diagnostics): bool {
            $diagnostics[] = $number . ': ' . $message;

            return true;
        });

        try {
            $statement();
            $error = null;
        } catch (Throwable $thrown) {
            $error = $thrown::class . ': ' . $thrown->getMessage();
        } finally {
            restore_error_handler();
        }

        return ['error' => $error, 'diagnostics' => $diagnostics];
    }
}

/**
 * An ArrayAccess container that counts how often it is asked to unset.
 */
final class Issue2702Bag implements ArrayAccess
{
    public array $items = ['k' => 1, 'j' => 2];

    public int $unsetCalls = 0;

    public function offsetExists(mixed $offset): bool
    {
        return isset($this->items[$offset]);
    }

    #[ReturnTypeWillChange]
    public function offsetGet(mixed $offset)
    {
        return $this->items[$offset] ?? null;
    }

    public function offsetSet(mixed $offset, mixed $value): void
    {
        if (null === $offset) {
            $this->items[] = $value;

            return;
        }

        $this->items[$offset] = $value;
    }

    public function offsetUnset(mixed $offset): void
    {
        ++$this->unsetCalls;

        unset($this->items[$offset]);
    }
}
