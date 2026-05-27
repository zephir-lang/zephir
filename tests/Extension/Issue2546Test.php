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

use ArrayIterator;
use PHPUnit\Framework\TestCase;
use Stub\Issue2546;

/**
 * Regression test for https://github.com/zephir-lang/zephir/issues/2546
 *
 * `continue` inside a `for ... in` loop over a PHP Iterator/Traversable object
 * was not calling next() on the iterator. The generated while(1) loop placed
 * next() at the *end* of the body; a C `continue` jumps to the while(1)
 * condition, skipping that call and replaying the same element forever.
 *
 * Fix: next() is now emitted at the *top* of the while(1) body, guarded by a
 * first-iteration flag (zend_bool), so `continue` correctly advances the
 * iterator before re-evaluating valid().
 */
final class Issue2546Test extends TestCase
{
    /**
     * `continue` on a middle element must skip that element and carry on.
     */
    public function testContinueSkipsMatchingElement(): void
    {
        $obj  = new Issue2546();
        $iter = new ArrayIterator([1, 2, 3, 4, 5]);

        $result = $obj->collectExcluding($iter, 3);

        $this->assertSame([1, 2, 4, 5], array_values($result));
    }

    /**
     * `continue` on the *first* element is the canonical infinite-loop trigger
     * from the bug report — the iterator must advance past element 1.
     */
    public function testContinueOnFirstElementAdvancesIterator(): void
    {
        $obj  = new Issue2546();
        $iter = new ArrayIterator([1, 2, 3]);

        $result = $obj->collectExcluding($iter, 1);

        $this->assertSame([2, 3], array_values($result));
    }

    /**
     * `continue` on every element must produce an empty result, not loop forever.
     */
    public function testContinueOnAllElementsProducesEmptyResult(): void
    {
        $obj  = new Issue2546();
        $iter = new ArrayIterator([7, 7, 7]);

        $result = $obj->collectExcluding($iter, 7);

        $this->assertSame([], array_values($result));
    }

    /**
     * `break` must still terminate the loop immediately (regression guard).
     */
    public function testBreakStillTerminatesLoop(): void
    {
        $obj  = new Issue2546();
        $iter = new ArrayIterator([1, 2, 3, 4, 5]);

        $result = $obj->collectUntil($iter, 3);

        $this->assertSame([1, 2], array_values($result));
    }

    /**
     * Normal iteration without any continue must be unaffected by the fix.
     */
    public function testNormalIterationWithoutContinue(): void
    {
        $obj  = new Issue2546();
        $iter = new ArrayIterator([10, 20, 30]);

        $result = $obj->collectExcluding($iter, 999);

        $this->assertSame([10, 20, 30], array_values($result));
    }

    /**
     * Key+value iteration with `continue` on a specific key.
     */
    public function testKeyValueIterationWithContinue(): void
    {
        $obj  = new Issue2546();
        $iter = new ArrayIterator(['a' => 1, 'b' => 2, 'c' => 3]);

        $result = $obj->collectKeysExcluding($iter, 'a');

        $this->assertSame(['b' => 2, 'c' => 3], $result);
    }

    /**
     * Key+value: skip the first key — exercises the continue path from the very
     * first element of the key-value object branch.
     */
    public function testKeyValueContinueOnFirstKey(): void
    {
        $obj  = new Issue2546();
        $iter = new ArrayIterator(['x' => 10, 'y' => 20, 'z' => 30]);

        $result = $obj->collectKeysExcluding($iter, 'x');

        $this->assertSame(['y' => 20, 'z' => 30], $result);
    }
}
