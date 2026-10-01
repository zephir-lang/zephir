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
use Stub\Issue2676;

use function extension_loaded;
use function gmp_init;

/**
 * PHP's `%` converts both operands to int, yields an int, and throws
 * TypeError for an operand it cannot treat as a number. Every assertion
 * compares the extension against the same PHP expression.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2676
 * @issue https://github.com/zephir-lang/zephir/issues/2677
 */
final class Issue2676Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2676 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2676();
    }

    public static function integerProvider(): array
    {
        return [
            'positive'         => [7, 3],
            'negative'         => [-7, 3],
            'negative divisor' => [7, -3],
            'zero dividend'    => [0, 5],
            'max'              => [PHP_INT_MAX, 10],
            'min by minus one' => [PHP_INT_MIN, -1],
            'modulo by zero'   => [7, 0],
        ];
    }

    /**
     * Operands that reach PHP's slow path: coercion, a warning or a TypeError.
     */
    public static function zvalProvider(): array
    {
        $cases = [
            'int'             => [7],
            'float'           => [7.5],
            'huge float'      => [1e20],
            'numeric string'  => ['7'],
            'float string'    => ['7.5'],
            'leading numeric' => ['5 apples'],
            'non-numeric'     => ['abc'],
            'empty string'    => [''],
            'null'            => [null],
            'true'            => [true],
            'false'           => [false],
            'array'           => [[1, 2]],
            'object'          => [new stdClass()],
        ];

        if (extension_loaded('gmp')) {
            $cases['gmp'] = [gmp_init(7)];
        }

        return $cases;
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
    public function testIntegerOperands(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modLongLong($a, $b), fn () => $a % $b);
        $this->assertMatchesPhp(fn () => $this->test->modLongVar($a, $b), fn () => $a % $b);
        $this->assertMatchesPhp(fn () => $this->test->modVarLong($a, $b), fn () => $a % $b);
        $this->assertMatchesPhp(fn () => $this->test->modVarVar($a, $b), fn () => $a % $b);
        $this->assertMatchesPhp(fn () => $this->test->modInferredLocal($a, $b), fn () => $a % $b);
    }

    public function testDoubleOperands(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modLongDouble(7, 2.5), fn () => 7 % 2.5);
        $this->assertMatchesPhp(fn () => $this->test->modDoubleLong(7.5, 2), fn () => 7.5 % 2);
        $this->assertMatchesPhp(fn () => $this->test->modDoubleDouble(7.0, 3.0), fn () => 7.0 % 3.0);
        $this->assertMatchesPhp(fn () => $this->test->modLongDouble(7, 0.5), fn () => 7 % 0.5);
    }

    public function testLiteralOperands(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modLiteral(), fn () => 7 % 3);
    }

    /**
     * @dataProvider zvalProvider
     */
    public function testVarDividend(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modVarLong($a, 4), fn () => $a % 4);
        $this->assertMatchesPhp(fn () => $this->test->modVarVar($a, 4), fn () => $a % 4);
        $this->assertMatchesPhp(fn () => $this->test->modVarDouble($a, 4.0), fn () => $a % 4.0);
        $this->assertMatchesPhp(fn () => $this->test->modVarLiteralDouble($a), fn () => $a % 2.5);
        $this->assertMatchesPhp(fn () => $this->test->modVarLiteralLong($a), fn () => $a % 4);
        $this->assertMatchesPhp(fn () => $this->test->modVarBool($a, true), fn () => $a % true);
        $this->assertMatchesPhp(fn () => $this->test->modVarByTrue($a), fn () => $a % true);
        $this->assertMatchesPhp(fn () => $this->test->modInferredLocal($a, 4), fn () => $a % 4);
    }

    /**
     * @dataProvider zvalProvider
     */
    public function testVarDivisor(mixed $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modLongVar(9, $b), fn () => 9 % $b);
        $this->assertMatchesPhp(fn () => $this->test->modVarVar(9, $b), fn () => 9 % $b);
        $this->assertMatchesPhp(fn () => $this->test->modDoubleVar(9.0, $b), fn () => 9.0 % $b);
        $this->assertMatchesPhp(fn () => $this->test->modBoolVar(true, $b), fn () => true % $b);
    }

    /**
     * A `long` local holds the int PHP's `%` yields, or stays 0 behind a
     * pending exception.
     *
     * @dataProvider zvalProvider
     */
    public function testLongLocalHoldsTheResult(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modTypedLong($a, 4), fn () => (int) ($a % 4));
    }

    /**
     * @dataProvider boolProvider
     */
    public function testBoolOperand(bool $flag): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modLongBool(5, $flag), fn () => 5 % $flag);
        $this->assertMatchesPhp(fn () => $this->test->modBoolLong($flag, 3), fn () => $flag % 3);
        $this->assertMatchesPhp(fn () => $this->test->modDoubleBool(7.0, $flag), fn () => 7.0 % $flag);
        $this->assertMatchesPhp(fn () => $this->test->modDoubleBool(5.5, $flag), fn () => 5.5 % $flag);
        $this->assertMatchesPhp(fn () => $this->test->modBoolDouble($flag, 3.0), fn () => $flag % 3.0);
        $this->assertMatchesPhp(fn () => $this->test->modBoolBool($flag, true), fn () => $flag % true);
        $this->assertMatchesPhp(fn () => $this->test->modBoolBool(true, $flag), fn () => true % $flag);
        $this->assertMatchesPhp(fn () => $this->test->modTrueByLong($flag ? 3 : 0), fn () => true % ($flag ? 3 : 0));
    }

    public function testLiteralTrueDivisor(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modLongByTrue(5), fn () => 5 % true);
    }

    public function testBoolMeetingANonNumericStringNamesTheBool(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modVarBool('abc', true), fn () => 'abc' % true);
        $this->assertMatchesPhp(fn () => $this->test->modBoolVar(true, 'abc'), fn () => true % 'abc');
    }

    /**
     * @dataProvider zvalProvider
     */
    public function testModAssignOnVar(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modAssignVarLong($a, 4), function () use ($a) {
            $x = $a;
            $x %= 4;

            return $x;
        });
        $this->assertMatchesPhp(fn () => $this->test->modAssignVarVar($a, 4), function () use ($a) {
            $x = $a;
            $x %= 4;

            return $x;
        });
    }

    /**
     * @dataProvider zvalProvider
     */
    public function testModAssignOnVarByVar(mixed $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modAssignVarVar(9, $b), function () use ($b) {
            $x = 9;
            $x %= $b;

            return $x;
        });
    }

    /**
     * @dataProvider integerProvider
     */
    public function testModAssignOnInferredLocal(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modAssignInferredLocal($b), function () use ($b) {
            $x = 42;
            $x %= $b;

            return $x;
        });
    }

    /**
     * @dataProvider integerProvider
     */
    public function testModAssignOnTypedLong(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->modAssignTypedLong($a, $b), fn () => $a % $b);
        $this->assertMatchesPhp(fn () => $this->test->modAssignTypedLongVar($a, $b), fn () => $a % $b);
    }

    /**
     * @dataProvider integerProvider
     */
    public function testModAssignOnTypedDouble(int $a, int $b): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->modAssignTypedDouble((float) $a + 0.5, $b),
            fn () => (float) (((float) $a + 0.5) % $b)
        );
    }

    /**
     * @dataProvider zvalProvider
     */
    public function testModAssignOnProperty(mixed $a): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->modAssignProperty((object) ['value' => $a], 4),
            fn () => ((object) ['value' => $a])->value % 4
        );
        $this->assertMatchesPhp(
            fn () => $this->test->modAssignPropertyVar((object) ['value' => $a], 4),
            fn () => ((object) ['value' => $a])->value % 4
        );
        $this->assertMatchesPhp(
            fn () => $this->test->modAssignPropertyLiteral((object) ['value' => $a]),
            fn () => ((object) ['value' => $a])->value % 4
        );
    }

    /**
     * @dataProvider zvalProvider
     */
    public function testModAssignOnPropertyByVar(mixed $b): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->modAssignPropertyVar((object) ['value' => 9], $b),
            fn () => 9 % $b
        );
    }

    /**
     * A C `/=` on a typed local raised SIGFPE for a zero divisor.
     *
     * @dataProvider integerProvider
     */
    public function testDivAssignOnTypedLocals(int $a, int $b): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divAssignTypedLong($a, $b), fn () => (int) ($a / $b));
        $this->assertMatchesPhp(
            fn () => $this->test->divAssignTypedDouble((float) $a, $b),
            fn () => (float) ((float) $a / $b)
        );
    }
}
