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

use PHPUnit\Framework\TestCase;
use Stub\Issue2698;

/**
 * Separating an array property hands the separated array to exactly one owner.
 *
 * kernel/object.c separates with ZVAL_DUP(), which yields a fresh array at
 * refcount 1 -- the single reference the local now holds, and the one the
 * trailing zval_ptr_dtor() releases. Three of the six separation sites dropped
 * that reference with Z_TRY_DELREF() first, so the dtor decremented zero
 * instead of freeing and the array, with every object inside it, was retained
 * for the rest of the request.
 *
 * It fires on the first write only: afterwards the property holds a refcount 1
 * array that updates in place. So exactly one array leaks per object, which is
 * how a request's whole service graph goes with it.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2698
 */
final class Issue2698Test extends TestCase
{
    private Issue2698 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2698();
    }

    /**
     * Asserted before every probe below. A flat reading from a probe that
     * cannot measure retention at all is indistinguishable from a pass.
     */
    public function testLeakProbeCanMeasureGrowth(): void
    {
        $this->subject->leakProbeControl(200);

        $this->assertGreaterThan(
            $this->subject->leakProbeControl(1000),
            $this->subject->leakProbeControl(50000),
            'The probe cannot measure retained memory, so the leak tests below are meaningless.'
        );
    }

    /**
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testOffsetWriteDoesNotLeakTheSeparatedArray(): void
    {
        // Settle interned strings and call caches before measuring.
        $this->subject->writeProbe(100);

        $this->assertSame(
            $this->subject->writeProbe(1000),
            $this->subject->writeProbe(50000),
            'Retained memory grows with the iteration count, so let this->items[key] leaks the separated array.'
        );
    }

    /**
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testNestedOffsetWriteDoesNotLeakTheSeparatedArray(): void
    {
        $this->subject->nestedProbe(100);

        $this->assertSame(
            $this->subject->nestedProbe(1000),
            $this->subject->nestedProbe(50000),
            'Retained memory grows with the iteration count, so let this->matrix[a][b] leaks the separated array.'
        );
    }

    /**
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testUnsetOffsetDoesNotLeakTheSeparatedArray(): void
    {
        $this->subject->unsetProbe(100);

        $this->assertSame(
            $this->subject->unsetProbe(1000),
            $this->subject->unsetProbe(50000),
            'Retained memory grows with the iteration count, so unset this->removable[key] leaks the separated array.'
        );
    }

    /**
     * The unset path again, through its refcounted branch: the first unset
     * separates, a local then shares the result, and the second unset
     * separates a refcounted array instead of the persistent default.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testUnsetOnASharedPropertyDoesNotLeakTheSeparatedArray(): void
    {
        $this->subject->unsetSharedProbe(100);

        $this->assertSame(
            $this->subject->unsetSharedProbe(1000),
            $this->subject->unsetSharedProbe(50000),
            'Retained memory grows with the iteration count, so unset on a shared array property leaks.'
        );
    }

    /**
     * This site was already correct: it puts the dropped reference back with
     * Z_ADDREF(). The test exists so that making the six separation sites look
     * alike cannot quietly break the one that was right.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testAppendDoesNotLeakTheSeparatedArray(): void
    {
        $this->subject->appendProbe(100);

        $this->assertSame(
            $this->subject->appendProbe(1000),
            $this->subject->appendProbe(50000),
            'Retained memory grows with the iteration count, so let this->bag[] leaks the separated array.'
        );
    }

    public function testWriteSeparatesFromTheSharedDefault(): void
    {
        $first = new Issue2698();
        $first->writeOffset('one', 1);

        $this->assertSame(['seed' => 1, 'one' => 1], $first->getItems());
        $this->assertSame(['seed' => 1], (new Issue2698())->getItems());
    }

    public function testNestedWriteSeparatesFromTheSharedDefault(): void
    {
        $first = new Issue2698();
        $first->writeNested('row', 'added', 2);
        $first->writeNested('fresh', 'deep', 3);

        $this->assertSame(
            ['row' => ['col' => 1, 'added' => 2], 'fresh' => ['deep' => 3]],
            $first->getMatrix()
        );
        $this->assertSame(['row' => ['col' => 1]], (new Issue2698())->getMatrix());
    }

    public function testUnsetSeparatesFromTheSharedDefault(): void
    {
        $first = new Issue2698();
        $first->removeOffset('a');

        $this->assertSame(['b' => 2], $first->getRemovable());
        $this->assertSame(['a' => 1, 'b' => 2], (new Issue2698())->getRemovable());
    }

    public function testAppendSeparatesFromTheSharedDefault(): void
    {
        $first = new Issue2698();
        $first->appendValue('added');

        $this->assertSame(['seed', 'added'], $first->getBag());
        $this->assertSame(['seed'], (new Issue2698())->getBag());
    }

    /**
     * The same construct written in plain PHP is the reference behaviour.
     */
    public function testMatchesPlainPhpSemantics(): void
    {
        $php = new class () {
            protected array $items     = ['seed' => 1];
            protected array $bag       = ['seed'];
            protected array $matrix    = ['row' => ['col' => 1]];
            protected array $removable = ['a' => 1, 'b' => 2];

            public function writeOffset(string $key, $value): void
            {
                $this->items[$key] = $value;
            }

            public function appendValue($value): void
            {
                $this->bag[] = $value;
            }

            public function writeNested(string $outer, string $inner, $value): void
            {
                $this->matrix[$outer][$inner] = $value;
            }

            public function removeOffset(string $key): void
            {
                unset($this->removable[$key]);
            }

            public function getItems(): array
            {
                return $this->items;
            }

            public function getBag(): array
            {
                return $this->bag;
            }

            public function getMatrix(): array
            {
                return $this->matrix;
            }

            public function getRemovable(): array
            {
                return $this->removable;
            }
        };

        $run = function ($subject, int $i): array {
            $subject->writeOffset('k'.$i, $i);
            $subject->appendValue('v'.$i);
            $subject->writeNested('row', 'n'.$i, $i);
            $subject->writeNested('o'.$i, 'deep', $i);
            $subject->removeOffset('a');

            return [$subject->getItems(), $subject->getBag(), $subject->getMatrix(), $subject->getRemovable()];
        };

        for ($i = 0; $i < 5; ++$i) {
            $this->assertSame($run(new $php(), $i), $run(new Issue2698(), $i));
        }

        $this->assertSame((new $php())->getItems(), (new Issue2698())->getItems());
        $this->assertSame((new $php())->getMatrix(), (new Issue2698())->getMatrix());
        $this->assertSame((new $php())->getRemovable(), (new Issue2698())->getRemovable());
    }
}
