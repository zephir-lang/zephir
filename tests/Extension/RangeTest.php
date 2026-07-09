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
use Stub\Range;

/**
 * `for` loops over the `..` / `...` range operators compile to an integer
 * counting loop (like `for i in range(...)`), instead of materialising an
 * intermediate array. This locks the runtime behaviour is unchanged.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2433
 */
final class RangeTest extends TestCase
{
    private Range $range;

    protected function setUp(): void
    {
        $this->range = new Range();
    }

    public function testInclusiveRangeLoopIteratesBothBounds(): void
    {
        $this->assertSame([0, 1, 2, 3, 4, 5], $this->range->loopInclusive());
    }

    public function testExclusiveRangeLoopPreservesInclusiveBehaviour(): void
    {
        // Zephir's `...` currently behaves inclusively (both operators desugar to
        // range(left, right)); the optimization must preserve that.
        $this->assertSame([0, 1, 2, 3, 4, 5], $this->range->loopExclusive());
    }

    public function testRangeOperatorMatchesRangeFunction(): void
    {
        $this->assertSame($this->range->loopRangeFn(), $this->range->loopInclusive());
    }

    public function testReverseRangeLoopCountsDown(): void
    {
        $this->assertSame([5, 4, 3, 2, 1, 0], $this->range->loopReverse());
    }

    public function testVariableBoundsRangeLoop(): void
    {
        $this->assertSame([3, 4, 5, 6, 7], $this->range->loopVarBounds(3, 7));
    }

    public function testCharRangeFallsBackToArrayPath(): void
    {
        // Char bounds are not int-compatible, so compileRange() bails and the
        // loop falls back to the generic array path; it must still work.
        $this->assertSame('abcde', $this->range->loopChars());
    }

    public function testInclusiveOperatorReturnsFullRangeArray(): void
    {
        $this->assertSame(range(0, 10), $this->range->inclusive1());
    }

    public function testExclusiveOperatorReturnsFullRangeArray(): void
    {
        $this->assertSame(range(0, 10), $this->range->exclusive1());
    }
}
