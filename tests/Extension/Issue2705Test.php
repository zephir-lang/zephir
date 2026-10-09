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
use Error;
use PHPUnit\Framework\TestCase;
use ReturnTypeWillChange;
use Stub\Issue2705;
use Throwable;
use stdClass;

/**
 * `unset` of an offset reached through a brace property name, a nested offset
 * or a static property, against what PHP does with the same statement.
 *
 * All of them fetched the container into a temporary and unset the key there,
 * so the key survived.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2705
 */
final class Issue2705Test extends TestCase
{
    private Issue2705 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2705();
    }

    /**
     * The issue's own reproduction.
     */
    public function testABraceLiteralNameRemovesTheKey(): void
    {
        $this->subject->container = ['' => 'empty key', 'k' => 1];

        $this->subject->unsetBraceLiteral();

        $this->assertSame(['' => 'empty key'], $this->subject->container);
    }

    public function testABraceVariableNameRemovesTheKey(): void
    {
        $this->subject->container = ['k' => 1, 'j' => 2];

        $this->subject->unsetBraceName('container');

        $this->assertSame(['j' => 2], $this->subject->container);
    }

    public function testANestedOffsetIsRemoved(): void
    {
        $this->subject->container = ['a' => ['b' => 1, 'c' => 2]];

        $this->subject->unsetNested();

        $this->assertSame(['a' => ['c' => 2]], $this->subject->container);
    }

    public function testAStaticOffsetIsRemoved(): void
    {
        Issue2705::$staticContainer = ['k' => 1, 'j' => 2];

        $this->subject->unsetStatic();

        $this->assertSame(['j' => 2], Issue2705::$staticContainer);
    }

    /**
     * Every shape against every container, asserted against PHP running the
     * same statement: what it throws, what it reports, and what is left. PHP
     * 8.0 is silent where 8.1 and later throw, so only a comparison is right on
     * every version.
     *
     * @dataProvider propertyShapes
     */
    public function testAPropertyShapeBehavesAsItDoesInPhp(string $method, array $arguments, callable $php): void
    {
        foreach (self::containers() as $label => $make) {
            $mirror            = new Issue2705Mirror();
            $mirror->container = $make();

            $expected = $this->outcomeOf(static function () use ($php, $mirror, $arguments): void {
                $php($mirror, ...$arguments);
            });

            $this->subject->container = $make();

            $actual = $this->outcomeOf(function () use ($method, $arguments): void {
                $this->subject->$method(...$arguments);
            });

            $this->assertSame($expected, $actual, "Container: $label");
            $this->assertEquals($mirror->container, $this->subject->container, "Container: $label");
        }
    }

    public static function propertyShapes(): array
    {
        return [
            'brace literal'          => ['unsetBraceLiteral', [], static function ($m) { unset($m->{'container'}['k']); }],
            'brace name'             => ['unsetBraceName', ['container'], static function ($m, $n) { unset($m->{$n}['k']); }],
            'brace name, var key'    => ['unsetBraceNameAndKey', ['container', 'j'], static function ($m, $n, $k) { unset($m->{$n}[$k]); }],
            'brace name, int key'    => ['unsetBraceNameAndKey', ['container', 3], static function ($m, $n, $k) { unset($m->{$n}[$k]); }],
            'brace name, long key'   => ['unsetBraceNameLongKey', ['container'], static function ($m, $n) { unset($m->{$n}[3]); }],
            'nested'                 => ['unsetNested', [], static function ($m) { unset($m->container['a']['b']); }],
            'nested by var'          => ['unsetNestedByVar', ['a', 'b'], static function ($m, $o, $i) { unset($m->container[$o][$i]); }],
            'nested, int keys'       => ['unsetNestedByVar', [3, 0], static function ($m, $o, $i) { unset($m->container[$o][$i]); }],
            'deep'                   => ['unsetDeep', [], static function ($m) { unset($m->container['a']['b']['c']); }],
            'brace literal, nested'  => ['unsetBraceLiteralNested', [], static function ($m) { unset($m->{'container'}['a']['b']); }],
            'brace name, nested'     => ['unsetBraceNameNested', ['container'], static function ($m, $n) { unset($m->{$n}['a']['b']); }],
        ];
    }

    public function testExecutionStopsWhenTheUnsetThrows(): void
    {
        $this->subject->container = 'hello';

        try {
            $this->subject->unsetNestedThenAssign();
            $this->fail('A nested unset on a string must throw.');
        } catch (Error) {
        }

        $this->assertSame('hello', $this->subject->container);
    }

    /**
     * The unset forms that predate the fix throw as well, and stop as well.
     *
     * @dataProvider throwingUnsets
     */
    public function testExecutionStopsWhenAnyUnsetThrows(string $method, callable $arguments, string $message): void
    {
        $this->subject->container = new Issue2705ThrowingBag();

        try {
            $this->subject->$method(...$arguments());
            $this->fail('The unset must throw.');
        } catch (Error $thrown) {
            $this->assertSame($message, $thrown->getMessage());
        }

        $this->assertInstanceOf(Issue2705ThrowingBag::class, $this->subject->container);
    }

    public static function throwingUnsets(): array
    {
        return [
            'property offset' => ['unsetPropertyOffsetThenAssign', static fn () => [], 'offsetUnset'],
            'local offset'    => ['unsetLocalOffsetThenAssign', static fn () => ['hello'], 'Cannot unset string offsets'],
            'property'        => ['unsetPropertyThenAssign', static fn () => [new Issue2705ThrowingBag()], '__unset'],
            'named property'  => ['unsetNamedPropertyThenAssign', static fn () => [new Issue2705ThrowingBag(), 'item'], '__unset'],
        ];
    }

    /**
     * @dataProvider localShapes
     */
    public function testALocalShapeBehavesAsItDoesInPhp(string $method, callable $php): void
    {
        foreach (self::containers() as $label => $make) {
            $expected = $this->outcomeOf(static function () use ($php, $make, &$phpResult): void {
                $phpResult = $php($make());
            });

            $actual = $this->outcomeOf(function () use ($method, $make, &$zephirResult): void {
                $zephirResult = $this->subject->$method($make());
            });

            $this->assertSame($expected, $actual, "Container: $label");
            $this->assertEquals($phpResult, $zephirResult, "Container: $label");
        }
    }

    public static function localShapes(): array
    {
        return [
            'nested' => ['unsetLocalNested', static function ($c) { unset($c['a']['b']); return $c; }],
            'deep'   => ['unsetLocalDeep', static function ($c) { unset($c['a']['b']['c']); return $c; }],
        ];
    }

    /**
     * @dataProvider staticShapes
     */
    public function testAStaticShapeBehavesAsItDoesInPhp(string $method, callable $php): void
    {
        foreach (self::containers() as $label => $make) {
            Issue2705Mirror::$staticContainer = $make();

            $expected = $this->outcomeOf(static function () use ($php): void {
                $php();
            });

            Issue2705::$staticContainer = $make();

            $actual = $this->outcomeOf(function () use ($method): void {
                $this->subject->$method();
            });

            $this->assertSame($expected, $actual, "Container: $label");
            $this->assertEquals(Issue2705Mirror::$staticContainer, Issue2705::$staticContainer, "Container: $label");
        }
    }

    public static function staticShapes(): array
    {
        return [
            'self'        => ['unsetStatic', static function () { unset(Issue2705Mirror::$staticContainer['k']); }],
            'nested'      => ['unsetStaticNested', static function () { unset(Issue2705Mirror::$staticContainer['a']['b']); }],
        ];
    }

    /**
     * PHP turns a non-string name into a string, and creates the property it
     * names, with the deprecation 8.2 added for that.
     *
     * @dataProvider oddNames
     */
    public function testANonStringNameBehavesAsItDoesInPhp(mixed $name): void
    {
        $mirror   = new Issue2705Mirror();
        $expected = $this->outcomeOf(static function () use ($mirror, $name): void {
            unset($mirror->{$name}['k']);
        });

        $actual = $this->outcomeOf(function () use ($name): void {
            $this->subject->unsetBraceName($name);
        });

        $this->assertSame($expected, $actual);
        $this->assertEquals(get_object_vars($mirror), $this->publicVars($this->subject));
    }

    public static function oddNames(): array
    {
        return [
            'int'           => [1],
            'array'         => [[]],
            'stringable'    => [new Issue2705Name()],
            'plain object'  => [new stdClass()],
        ];
    }

    /**
     * The array the property shares with someone else must not change; a
     * property bound by reference must, because the reference is the property.
     */
    public function testASharedArrayIsSeparated(): void
    {
        $source = ['k' => 1, 'a' => ['b' => 1, 'c' => 2]];

        $this->subject->container = $source;
        $this->subject->unsetBraceName('container');
        $this->subject->unsetNested();

        $this->assertSame(['k' => 1, 'a' => ['b' => 1, 'c' => 2]], $source);
        $this->assertSame(['a' => ['c' => 2]], $this->subject->container);
    }

    public function testASharedInnerArrayIsSeparated(): void
    {
        $inner  = ['b' => 1, 'c' => 2];
        $source = ['a' => $inner];

        $this->subject->container = $source;
        $this->subject->unsetNested();

        $this->assertSame(['b' => 1, 'c' => 2], $inner);
        $this->assertSame(['a' => $inner], $source);
        $this->assertSame(['a' => ['c' => 2]], $this->subject->container);
    }

    public function testAnUnsetReachesThroughAReference(): void
    {
        $bound                    = ['k' => 1, 'a' => ['b' => 1, 'c' => 2]];
        $this->subject->container = &$bound;

        $this->subject->unsetBraceName('container');
        $this->subject->unsetNested();

        $this->assertSame(['a' => ['c' => 2]], $bound);
    }

    public function testAStaticSharedArrayIsSeparated(): void
    {
        $source                     = ['k' => 1, 'a' => ['b' => 1, 'c' => 2]];
        Issue2705::$staticContainer = $source;

        $this->subject->unsetStatic();
        $this->subject->unsetStaticNested();

        $this->assertSame(['k' => 1, 'a' => ['b' => 1, 'c' => 2]], $source);
        $this->assertSame(['a' => ['c' => 2]], Issue2705::$staticContainer);
    }

    /**
     * An ArrayAccess container receives one offsetUnset(), whichever way the
     * property was named.
     */
    public function testAnArrayAccessPropertyReceivesOneOffsetUnset(): void
    {
        $bag                      = new Issue2705Bag();
        $this->subject->container = $bag;

        $this->subject->unsetBraceName('container');

        $this->assertSame(1, $bag->unsetCalls);
        $this->assertSame(['j' => 2, 'a' => ['b' => 1]], $bag->items);
    }

    public function testAnArrayAccessElementReceivesOneOffsetUnset(): void
    {
        $bag                      = new Issue2705Bag();
        $this->subject->container = ['a' => $bag];

        $this->subject->unsetNestedByVar('a', 'k');

        $this->assertSame(1, $bag->unsetCalls);
        $this->assertSame(['j' => 2, 'a' => ['b' => 1]], $bag->items);
    }

    /**
     * The persistent default must not be reachable from a copy taken before,
     * and a copy taken after must not alias the property.
     */
    public function testTheUnsetsSeparateFromTheDefaultAndLeaveNoReference(): void
    {
        $this->assertSame(
            [
                ['k' => 1, 'a' => ['b' => 1, 'c' => 2]],
                ['a' => ['c' => 2]],
                ['a' => ['c' => 9]],
            ],
            $this->subject->snapshotDefaults()
        );
    }

    public function testTheStaticUnsetsSeparateFromTheDefault(): void
    {
        $this->assertSame(
            [
                ['k' => 1, 'a' => ['b' => 1, 'c' => 2]],
                ['a' => ['c' => 2]],
            ],
            $this->subject->snapshotStaticDefaults()
        );
    }

    /**
     * @dataProvider probes
     */
    public function testNothingLeaks(string $probe, array $arguments): void
    {
        $this->subject->$probe(100, ...$arguments);

        $this->assertSame(
            $this->subject->$probe(1000, ...$arguments),
            $this->subject->$probe(50000, ...$arguments),
            'Retained memory grows with the iteration count.'
        );
    }

    public static function probes(): array
    {
        return [
            'nested'     => ['nestedProbe', []],
            'static'     => ['staticProbe', []],
            'brace name' => ['braceNameProbe', ['container']],
        ];
    }

    /**
     * Every kind of container, and every kind of thing at `a`.
     *
     * @return array<string, callable>
     */
    private static function containers(): array
    {
        return [
            'array'              => static fn () => ['k' => 1, 'j' => 2, 3 => [0 => 'zero', 1 => 'one'], 'a' => ['b' => ['c' => 1, 'd' => 2], 'c' => 2]],
            'array, no a'        => static fn () => ['k' => 1],
            'a is null'          => static fn () => ['a' => null],
            'a is int'           => static fn () => ['a' => 5],
            'a is false'         => static fn () => ['a' => false],
            'a is string'        => static fn () => ['a' => 'abc'],
            'a is array access'  => static fn () => ['a' => new Issue2705Bag()],
            'a is plain object'  => static fn () => ['a' => new stdClass()],
            'array access'       => static fn () => new Issue2705Bag(),
            'plain object'       => static fn () => new stdClass(),
            'string'             => static fn () => 'hello',
            'int'                => static fn () => 5,
            'false'              => static fn () => false,
            'null'               => static fn () => null,
        ];
    }

    private function publicVars(object $object): array
    {
        return get_object_vars($object);
    }

    /**
     * @return array{error: string|null, diagnostics: array<int, string>}
     */
    private function outcomeOf(callable $statement): array
    {
        $diagnostics = [];

        set_error_handler(static function (int $number, string $message) use (&$diagnostics): bool {
            $diagnostics[] = $number . ': ' . str_replace([Issue2705Mirror::class, Issue2705::class], 'CLASS', $message);

            return true;
        });

        try {
            $statement();
            $error = null;
        } catch (Throwable $thrown) {
            $error = $thrown::class . ': '
                . str_replace([Issue2705Mirror::class, Issue2705::class], 'CLASS', $thrown->getMessage());
        } finally {
            restore_error_handler();
        }

        return ['error' => $error, 'diagnostics' => $diagnostics];
    }
}

/**
 * A PHP class with the same public surface, to run the same statement on.
 */
final class Issue2705Mirror
{
    public $container;

    public static $staticContainer;
}

/**
 * An ArrayAccess container that counts how often it is asked to unset.
 */
final class Issue2705Bag implements ArrayAccess
{
    public array $items = ['k' => 1, 'j' => 2, 'a' => ['b' => 1]];

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

/**
 * Throws from every way an unset can reach it.
 */
final class Issue2705ThrowingBag implements ArrayAccess
{
    public function offsetExists(mixed $offset): bool
    {
        return true;
    }

    #[ReturnTypeWillChange]
    public function offsetGet(mixed $offset)
    {
        return null;
    }

    public function offsetSet(mixed $offset, mixed $value): void
    {
    }

    public function offsetUnset(mixed $offset): void
    {
        throw new Error('offsetUnset');
    }

    public function __unset(string $name): void
    {
        throw new Error('__unset');
    }
}

final class Issue2705Name
{
    public function __toString(): string
    {
        return 'container';
    }
}
