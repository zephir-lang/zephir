<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\UnspacedMinus;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2011
 */
final class UnspacedMinusTest extends TestCase
{
    private UnspacedMinus $test;

    protected function setUp(): void
    {
        $this->test = new UnspacedMinus();
    }

    public function testRangeWithGluedMinus(): void
    {
        $this->assertSame([0, 1, 2], $this->test->rangeLen(3));
    }

    public function testGluedMinus(): void
    {
        $this->assertSame(41, $this->test->glued(42));
    }

    public function testSpaceBeforeMinus(): void
    {
        $this->assertSame(41, $this->test->spaceBefore(42));
    }

    public function testArrayIndexMinus(): void
    {
        $this->assertSame(9, $this->test->arrayIndex([10]));
    }

    public function testNegativeLiteralUnchanged(): void
    {
        $this->assertSame(-1, $this->test->negativeLiteral());
    }

    public function testGluedAdd(): void
    {
        $this->assertSame(43, $this->test->gluedAdd(42));
    }

    public function testGluedMul(): void
    {
        $this->assertSame(84, $this->test->gluedMul(42));
    }

    public function testGluedDiv(): void
    {
        $this->assertSame(21.0, $this->test->gluedDiv(42));
    }

    public function testGluedMod(): void
    {
        $this->assertSame(2, $this->test->gluedMod(42));
    }

    public function testMulByNegativeLiteral(): void
    {
        $this->assertSame(-42, $this->test->mulByNegative(42));
    }

    public function testChainedPrecedence(): void
    {
        // a*2-1 => (a*2)-1
        $this->assertSame(83, $this->test->chained(42));
    }
}
