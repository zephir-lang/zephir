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
use Stub\Issue2394;

/**
 * @see https://github.com/zephir-lang/zephir/issues/2394
 */
final class Issue2394Test extends TestCase
{
    private Issue2394 $sut;

    protected function setUp(): void
    {
        $this->sut = new Issue2394();
    }

    public function testAdditionDefault(): void
    {
        $this->assertSame(5, $this->sut->addition(4));
    }

    public function testSubtractionDefault(): void
    {
        $this->assertSame(3, $this->sut->subtraction(4));
    }

    public function testMultiplicationDefault(): void
    {
        $this->assertSame(8, $this->sut->multiplication(4));
    }

    public function testDivisionDefault(): void
    {
        $this->assertSame(2.0, $this->sut->division(4));
    }

    public function testModuloDefault(): void
    {
        $this->assertSame(1, $this->sut->modulo(4));
    }

    public function testConcatenationDefault(): void
    {
        $this->assertSame('ab', $this->sut->concatenation('a', 'b'));
    }

    public function testTernaryDefault(): void
    {
        $this->assertSame(10, $this->sut->ternary(1));
        $this->assertSame(20, $this->sut->ternary(5));
    }

    public function testComparisonDefault(): void
    {
        $this->assertTrue($this->sut->comparison(5));
        $this->assertFalse($this->sut->comparison(1));
    }

    public function testMethodCallDefault(): void
    {
        $this->assertSame(7, $this->sut->methodCall());
    }

    public function testBackslashLoopFromIssue(): void
    {
        // "a\bc" -> 'a', then escape consumes 'b' literally, then 'c'.
        $this->assertSame('abc', $this->sut->backslashLoop('a\\bc'));
    }
}
