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
use Stub\Concat;

final class ConcatTest extends TestCase
{
    private Concat $test;

    protected function setUp(): void
    {
        $this->test = new Concat();
    }

    public function testShouldConcatenateLocalVariableAndSelfVariable(): void
    {
        $this->assertNull($this->test->getTestProperty());

        $this->test->testConcatBySelfProperty('test');
        $this->assertSame('test', $this->test->getTestProperty());

        $this->test->testConcatBySelfProperty(' string');
        $this->assertSame('test string', $this->test->getTestProperty());
    }

    public function testShouldAppendVariableToString(): void
    {
        $this->assertSame('appendtest', $this->test->testConcat1());
    }

    public function testShouldConcatenateVariableWithString(): void
    {
        $this->assertSame('appendotherappendother', $this->test->testConcat2());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1573
     */
    public function testShouldConcatenateStringsSimilarToIntegersNumbers(): void
    {
        $this->assertSame('21', $this->test->testConcat3());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1893
     */
    public function testShouldConcatenateStringWithVarDouble(): void
    {
        $this->assertSame(
            'SELECT * FROM TEST WHERE value <= 946.5 AND value >= 473.25',
            $this->test->testConcat4(1893)
        );
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1893
     */
    public function testShouldConcatenateStringWithDouble(): void
    {
        $this->assertSame(
            'Concatenated string with number 18.93000001',
            $this->test->testConcat5(18.93000001)
        );
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2660
     */
    public function testShouldConcatenateStringWithIntVariable(): void
    {
        foreach ([0, 5, -5, 9, 10, PHP_INT_MAX, PHP_INT_MIN] as $number) {
            $this->assertSame('n=' . $number, $this->test->testConcatStringWithIntVar($number));
        }
    }

    /**
     * Zephir's `long` is a C `long`, so it is 32-bit on Windows LLP64 while
     * `int` is a 64-bit `zend_long` everywhere. The 64-bit range is asserted on
     * the `int` variant above; this one only has to prove the `long` operand
     * reaches the same helper.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2660
     * @see   https://github.com/zephir-lang/zephir/issues/2666
     */
    public function testShouldConcatenateStringWithLongVariable(): void
    {
        $this->assertSame('n=' . 2147483647, $this->test->testConcatStringWithLongVar());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2660
     */
    public function testShouldConcatenateStringWithUnsignedVariables(): void
    {
        $this->assertSame('n=5', $this->test->testConcatStringWithUintVar());
        $this->assertSame('n=5', $this->test->testConcatStringWithUlongVar());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2660
     */
    public function testShouldConcatenateStringWithCharVariable(): void
    {
        $this->assertSame('n=A', $this->test->testConcatStringWithCharVar());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2660
     */
    public function testShouldConcatenateVarWithIntVariable(): void
    {
        foreach ([0, 5, -5, 9, 10, PHP_INT_MAX, PHP_INT_MIN] as $number) {
            $this->assertSame('n=' . $number, $this->test->testConcatVarWithIntVar($number));
        }
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2660
     */
    public function testShouldConcatenateVarWithCharVariable(): void
    {
        $this->assertSame('n=A', $this->test->testConcatVarWithCharVar());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2660
     */
    public function testShouldConcatenateVarWithIntLiteral(): void
    {
        $this->assertSame('n=5', $this->test->testConcatVarWithIntLiteral());
    }
}
