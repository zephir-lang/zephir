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
use Stub\Chars;

final class CharsTest extends TestCase
{
    private Chars $test;

    protected function setUp(): void
    {
        $this->test = new Chars();
    }

    public function testSumChars(): void
    {
        $this->assertSame(\chr($this->test->sumChars1()), 'a');
        $this->assertSame(\chr($this->test->sumChars2('A')), 'a');
    }

    public function testDiffChars(): void
    {
        $this->assertSame(\chr($this->test->diffChars1()), 'A');
        $this->assertSame(\chr($this->test->diffChars2('a')), 'A');
    }

    public function testSumOverflowHex(): void
    {
        // 32 + 64 + 42 = 138 = 0x8A
        $this->assertSame('8A', $this->test->sumOverflowHex());
    }

    public function testSumOverflowValue(): void
    {
        // 32 + 64 + 42 = 138, must not sign-extend to a negative long
        $this->assertSame(138, $this->test->sumOverflowValue());
    }

    public function testSumCharParamOverflow(): void
    {
        // char parameter (150) + 100 = 250, byte value preserved
        $this->assertSame(250, $this->test->sumCharParamOverflow(\chr(150)));
    }

    public function testCharUnderflow(): void
    {
        // 0 - 1 wraps around to the byte value 255 (not -1)
        $this->assertSame(255, $this->test->charUnderflow());
    }

    public function testMaxByteValue(): void
    {
        // 0 + 255 = 255, the maximum byte value
        $this->assertSame(255, $this->test->maxByteValue());
    }

    public function testSumUchar(): void
    {
        // uchar 'd' (100) + 100 = 200
        $this->assertSame(200, $this->test->sumUchar());
    }
}
