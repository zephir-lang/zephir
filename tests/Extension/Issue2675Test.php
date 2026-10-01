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
use stdClass;
use Stub\Issue2675;

/**
 * PHP's `/` narrows an exact integer quotient to `int`, returns a `float`
 * otherwise, and throws TypeError for an operand it cannot treat as a number.
 * Every assertion compares the extension against the same PHP expression.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2675
 * @issue https://github.com/zephir-lang/zephir/issues/2676
 * @issue https://github.com/zephir-lang/zephir/issues/2677
 */
final class Issue2675Test extends TestCase
{
    use AssertsPhpParity;

    /**
     * 2^53 + 1 is the first integer a double cannot hold, so dividing
     * 3 * (2^53 + 1) by 3 tells an exact integer quotient from a double one.
     */
    private const BEYOND_DOUBLE = 9007199254740993;

    private Issue2675 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2675();
    }

    public static function integerProvider(): array
    {
        return [
            'exact'             => [4, 2],
            'inexact'           => [7, 2],
            'negative exact'    => [-6, 3],
            'negative inexact'  => [-7, 2],
            'zero dividend'     => [0, 5],
            'max by one'        => [PHP_INT_MAX, 1],
            'max by itself'     => [PHP_INT_MAX, PHP_INT_MAX],
            'beyond double'     => [3 * self::BEYOND_DOUBLE, 3],
            'min by minus one'  => [PHP_INT_MIN, -1],
            'division by zero'  => [7, 0],
        ];
    }

    /**
     * Operands that reach PHP's slow path: coercion, a warning or a TypeError.
     */
    public static function zvalProvider(): array
    {
        return [
            'int'             => [6],
            'float'           => [2.5],
            'numeric string'  => ['6'],
            'float string'    => ['6.0'],
            'leading numeric' => ['5 apples'],
            'non-numeric'     => ['abc'],
            'null'            => [null],
            'true'            => [true],
            'false'           => [false],
            'array'           => [[]],
            'object'          => [new stdClass()],
        ];
    }

    public static function boolProvider(): array
    {
        return [
            'true'  => [true],
            'false' => [false],
        ];
    }

    /**
     * @dataProvider integerProvider
     */
    public function testLongByLong(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divLongLong($a, $b), fn () => $a / $b);
    }

    /**
     * @dataProvider integerProvider
     */
    public function testLongByVar(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divLongVar($a, $b), fn () => $a / $b);
    }

    /**
     * @dataProvider integerProvider
     */
    public function testVarByLong(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divVarLong($a, $b), fn () => $a / $b);
    }

    /**
     * @dataProvider integerProvider
     */
    public function testVarByVar(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divVarVar($a, $b), fn () => $a / $b);
    }

    /**
     * @dataProvider integerProvider
     */
    public function testInferredLocalKeepsTheIntResult(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divInferredLocal($a, $b), fn () => $a / $b);
    }

    /**
     * @dataProvider integerProvider
     */
    public function testChainedDivisionKeepsTheIntResult(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divChained($a, $b), fn () => ($a / $b) * 2);
    }

    /**
     * PHP coerces the int quotient to the declared float, so the exact
     * quotient is computed first and converted once.
     *
     * @dataProvider integerProvider
     */
    public function testDoubleReturnTypeIsTheCoercedQuotient(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divReturnDouble($a, $b), fn () => (float) ($a / $b));
    }

    /**
     * @dataProvider integerProvider
     */
    public function testDoubleLocalIsTheCoercedQuotient(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divTypedDouble($a, $b), fn () => (float) ($a / $b));
    }

    public function testLongLocalTruncatesTheQuotient(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divTypedLong(4, 2), fn () => (int) (4 / 2));
        $this->assertMatchesPhp(fn () => $this->test->divTypedLong(7, 2), fn () => (int) (7 / 2));
        $this->assertMatchesPhp(fn () => $this->test->divTypedLong(-7, 2), fn () => (int) (-7 / 2));
        $this->assertMatchesPhp(fn () => $this->test->divTypedLong(7, 0), fn () => (int) (7 / 0));
    }

    public function testDoubleOperandsAlwaysReturnFloat(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divLongDouble(4, 2.0), fn () => 4 / 2.0);
        $this->assertMatchesPhp(fn () => $this->test->divDoubleLong(4.0, 2), fn () => 4.0 / 2);
        $this->assertMatchesPhp(fn () => $this->test->divDoubleDouble(4.0, 2.0), fn () => 4.0 / 2.0);
        $this->assertMatchesPhp(fn () => $this->test->divLongDouble(4, 0.0), fn () => 4 / 0.0);
    }

    public function testLiteralOperands(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divLiteralExact(), fn () => 4 / 2);
        $this->assertMatchesPhp(fn () => $this->test->divLiteralInexact(), fn () => 7 / 2);
    }

    /**
     * @dataProvider zvalProvider
     */
    public function testVarDividend(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divVarLong($a, 2), fn () => $a / 2);
        $this->assertMatchesPhp(fn () => $this->test->divVarVar($a, 2), fn () => $a / 2);
        $this->assertMatchesPhp(fn () => $this->test->divVarDouble($a, 2.0), fn () => $a / 2.0);
        $this->assertMatchesPhp(fn () => $this->test->divVarLiteralDouble($a), fn () => $a / 2.0);
        $this->assertMatchesPhp(fn () => $this->test->divVarBool($a, true), fn () => $a / true);
    }

    /**
     * @dataProvider zvalProvider
     */
    public function testVarDivisor(mixed $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divLongVar(6, $b), fn () => 6 / $b);
        $this->assertMatchesPhp(fn () => $this->test->divVarVar(6, $b), fn () => 6 / $b);
        $this->assertMatchesPhp(fn () => $this->test->divDoubleVar(6.0, $b), fn () => 6.0 / $b);
        $this->assertMatchesPhp(fn () => $this->test->divBoolVar(true, $b), fn () => true / $b);
    }

    /**
     * @dataProvider boolProvider
     */
    public function testBoolDivisor(bool $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divLongBool(5, $b), fn () => 5 / $b);
        $this->assertMatchesPhp(fn () => $this->test->divVarBool(5, $b), fn () => 5 / $b);
        $this->assertMatchesPhp(fn () => $this->test->divDoubleBool(5.0, $b), fn () => 5.0 / $b);
        $this->assertMatchesPhp(fn () => $this->test->divBoolBool(true, $b), fn () => true / $b);
    }

    /**
     * @dataProvider boolProvider
     */
    public function testBoolDividend(bool $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divBoolLong($a, 1), fn () => $a / 1);
        $this->assertMatchesPhp(fn () => $this->test->divBoolLong($a, 2), fn () => $a / 2);
        $this->assertMatchesPhp(fn () => $this->test->divBoolLong($a, 0), fn () => $a / 0);
        $this->assertMatchesPhp(fn () => $this->test->divBoolDouble($a, 2.0), fn () => $a / 2.0);
    }

    public function testLiteralBoolOperands(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divLongByTrue(5), fn () => 5 / true);
        $this->assertMatchesPhp(fn () => $this->test->divTrueByLong(2), fn () => true / 2);
        $this->assertMatchesPhp(fn () => $this->test->divTrueByLong(1), fn () => true / 1);
        $this->assertMatchesPhp(fn () => $this->test->divTrueByLong(0), fn () => true / 0);
    }

    /**
     * @dataProvider integerProvider
     */
    public function testDivAssignOnVar(int $a, int $b): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->divAssignVarLong($a, $b),
            function () use ($a, $b) {
                $x = $a;
                $x /= $b;

                return $x;
            }
        );
        $this->assertMatchesPhp(
            fn () => $this->test->divAssignVarVar($a, $b),
            function () use ($a, $b) {
                $x = $a;
                $x /= $b;

                return $x;
            }
        );
    }

    /**
     * @dataProvider zvalProvider
     */
    public function testDivAssignOnVarWithVarDivisor(mixed $b): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->divAssignVarVar(6, $b),
            function () use ($b) {
                $x = 6;
                $x /= $b;

                return $x;
            }
        );
    }

    public function testDivAssignOnInferredLocal(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divAssignInferredLocal(2), fn () => 42 / 2);
        $this->assertMatchesPhp(fn () => $this->test->divAssignInferredLocal(4), fn () => 42 / 4);
        $this->assertMatchesPhp(fn () => $this->test->divAssignInferredLocal(0), fn () => 42 / 0);
    }

    /**
     * @dataProvider integerProvider
     */
    public function testDivAssignOnProperty(int $a, int $b): void
    {
        $php = function () use ($a, $b) {
            $obj        = new stdClass();
            $obj->value = $a;
            $obj->value /= $b;

            return $obj->value;
        };

        $this->assertMatchesPhp(fn () => $this->test->divAssignProperty($this->holding($a), $b), $php);
        $this->assertMatchesPhp(fn () => $this->test->divAssignPropertyVar($this->holding($a), $b), $php);
    }

    public function testDivAssignOnPropertyWithLiteralDivisor(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divAssignPropertyLiteral($this->holding(4)), fn () => 4 / 2);
        $this->assertMatchesPhp(fn () => $this->test->divAssignPropertyLiteral($this->holding(7)), fn () => 7 / 2);
    }

    private function holding(mixed $value): stdClass
    {
        $obj        = new stdClass();
        $obj->value = $value;

        return $obj;
    }
}
