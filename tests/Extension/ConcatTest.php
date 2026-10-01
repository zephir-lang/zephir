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
     * `long` is a `zend_long` on every platform since #2666, so it carries the
     * same 64-bit range as `int`. Before that it was a C `long`, 32-bit under
     * Windows LLP64, and PHP_INT_MAX came back as `n=-1` there.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2660
     * @issue https://github.com/zephir-lang/zephir/issues/2666
     */
    public function testShouldConcatenateStringWithLongVariable(): void
    {
        foreach ([0, 5, -5, 2147483647, PHP_INT_MAX, PHP_INT_MIN] as $number) {
            $this->assertSame('n=' . $number, $this->test->testConcatStringWithLongVar($number));
        }
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

    /**
     * PHP renders a float with `zend_double_to_str()`, which reads the
     * `precision` ini at run time, so the expectation is the same expression
     * evaluated by PHP in this process rather than a hand-written table. That
     * is what catches a `printf`-based implementation.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldConcatenateStringWithDoubleVariable(): void
    {
        foreach (self::doubles() as $number) {
            $this->assertSame('n=' . $number, $this->test->testConcatStringWithDoubleVar($number));
        }
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldConcatenateVarWithDoubleVariable(): void
    {
        foreach (self::doubles() as $number) {
            $this->assertSame('n=' . $number, $this->test->testConcatVarWithDoubleVar($number));
        }
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldConcatenateWithDoubleLiteral(): void
    {
        $this->assertSame('n=' . 1.5, $this->test->testConcatStringWithDoubleLiteral());
        $this->assertSame('n=' . 1.5, $this->test->testConcatVarWithDoubleLiteral());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldConcatenateWithBoolVariable(): void
    {
        $this->assertSame('n=' . true, $this->test->testConcatStringWithBoolVar(true));
        $this->assertSame('n=' . false, $this->test->testConcatStringWithBoolVar(false));
        $this->assertSame('n=' . true, $this->test->testConcatVarWithBoolVar(true));
        $this->assertSame('n=' . false, $this->test->testConcatVarWithBoolVar(false));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldConcatenateWithBoolLiteral(): void
    {
        $this->assertSame('n=' . true, $this->test->testConcatStringWithTrueLiteral());
        $this->assertSame('n=' . false, $this->test->testConcatStringWithFalseLiteral());
        $this->assertSame('n=' . true, $this->test->testConcatVarWithTrueLiteral());
        $this->assertSame('n=' . false, $this->test->testConcatVarWithFalseLiteral());
    }

    /**
     * Appending nothing is not the same as doing nothing: the left operand is
     * still converted, so an integer comes back as a string.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldConvertTheLeftOperandWhenNothingIsAppended(): void
    {
        $this->assertSame(5 . false, $this->test->testConcatIntVarWithFalseLiteral());
        $this->assertSame(5 . null, $this->test->testConcatVarWithNullLiteral());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldAppendEveryLiteralTypeToAnObjectProperty(): void
    {
        $this->test->resetStore();
        $this->test->appendPropertyLiterals();

        $this->assertSame('s' . 5 . 1.5 . true . false . null, $this->test->getStore());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldAppendEveryVariableTypeToAnObjectProperty(): void
    {
        $this->test->resetStore();
        $this->test->appendPropertyVariables(5, 1.5, true, 'tail');

        $this->assertSame(5 . 1.5 . true . 'A' . 'tail', $this->test->getStore());
    }

    /**
     * PHP raises "Array to string conversion" and appends "Array".
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldAppendAnArrayToAnObjectPropertyTheWayPhpDoes(): void
    {
        $warnings = [];
        set_error_handler(
            static function (int $number, string $message) use (&$warnings): bool {
                $warnings[] = $message;

                return true;
            },
            E_WARNING
        );

        try {
            $this->test->resetStore();
            $this->test->appendPropertyArray();
        } finally {
            restore_error_handler();
        }

        $this->assertSame('Array', $this->test->getStore());
        $this->assertSame(['Array to string conversion'], $warnings);
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldAppendEveryLiteralTypeToAStaticProperty(): void
    {
        Concat::resetAppended();
        Concat::appendStaticLiterals();

        $this->assertSame('s' . 5 . 1.5 . true . false, Concat::getAppended());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldAppendEveryVariableTypeToAStaticProperty(): void
    {
        Concat::resetAppended();
        Concat::appendStaticVariables(5, 1.5, true);

        $this->assertSame(5 . 1.5 . true, Concat::getAppended());
    }

    /**
     * The binary `.` operator rejected a `bool` and a `char` operand although
     * `.=` accepted both.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldConcatenateExpressionWithBoolAndCharOperands(): void
    {
        $this->assertSame('n=' . true, $this->test->testConcatExprWithBoolVar(true));
        $this->assertSame('n=' . false, $this->test->testConcatExprWithBoolVar(false));
        $this->assertSame('n=' . true, $this->test->testConcatExprWithTrueLiteral());
        $this->assertSame('n=' . false, $this->test->testConcatExprWithFalseLiteral());
        $this->assertSame('n=A', $this->test->testConcatExprWithCharVar());
        $this->assertSame('n=A', $this->test->testConcatExprWithCharLiteral());
    }

    /**
     * A local left-hand side used to reject an array operand outright while an
     * object property accepted it. Both now do what PHP does.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2664
     */
    public function testShouldConcatenateAnArrayOntoALocalTheWayPhpDoes(): void
    {
        $this->assertSame('n=Array', @('n=' . [1, 2]), 'PHP renders an array operand as "Array".');

        $warnings = [];
        set_error_handler(
            static function (int $number, string $message) use (&$warnings): bool {
                $warnings[] = $message;

                return true;
            },
            E_WARNING
        );

        try {
            $results = [
                $this->test->testConcatStringWithArrayLiteral(),
                $this->test->testConcatStringWithArrayVar(),
                $this->test->testConcatVarWithArrayLiteral(),
                $this->test->testConcatVarWithArrayVar(),
            ];
        } finally {
            restore_error_handler();
        }

        $this->assertSame(['n=Array', 'n=Array', 'n=Array', 'n=Array'], $results);
        $this->assertSame(array_fill(0, 4, 'Array to string conversion'), $warnings);
    }

    /**
     * @return list<float>
     */
    private static function doubles(): array
    {
        return [
            1.5,
            -1.5,
            0.0,
            -0.0,
            2.0,
            1 / 3,
            1e10,
            1e100,
            PHP_FLOAT_EPSILON,
            PHP_FLOAT_MIN,
            PHP_FLOAT_MAX,
        ];
    }
}
