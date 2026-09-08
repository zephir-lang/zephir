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
use Stub\Issue2666;

/**
 * A PHP integer is a `zend_long`, i.e. `int64_t`, on every 64-bit target.
 * Zephir's `long`/`ulong` mapped to a C `long`/`unsigned long`, which is
 * 32-bit under LLP64 (Windows x64), so the top half of every value was
 * dropped there while Linux and macOS saw nothing wrong.
 *
 * These assertions pass on LP64 before and after the fix -- they exist to fail
 * on the Windows CI legs, which is where the bug lives. The generated C is
 * checked from any platform by Zephir\Test\BlackBox\Issue2666Test and
 * Zephir\Test\KernelPortabilityTest.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2666
 */
final class Issue2666Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2666 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2666();
    }

    /**
     * Values that survive a 32-bit `long` and values that do not.
     */
    public static function sixtyFourBitProvider(): array
    {
        return [
            'zero'           => [0],
            'one'            => [1],
            'minus one'      => [-1],
            'int32 max'      => [2147483647],
            'int32 min'      => [-2147483648],
            'above int32'    => [4294967296],
            'below int32'    => [-4294967297],
            'int64 max'      => [PHP_INT_MAX],
            'int64 min'      => [PHP_INT_MIN],
        ];
    }

    public static function unsignedProvider(): array
    {
        return [
            'zero'        => [0],
            'one'         => [1],
            'uint32 max'  => [4294967295],
            'above int32' => [4294967296],
            'int64 max'   => [PHP_INT_MAX],
        ];
    }

    /**
     * @dataProvider sixtyFourBitProvider
     */
    public function testLongParameterKeepsAllSixtyFourBits(int $number): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->longParamToString($number),
            fn () => 'n=' . $number
        );
    }

    /**
     * Control: `int` was already a `zend_long` on every platform.
     *
     * @dataProvider sixtyFourBitProvider
     */
    public function testIntParameterKeepsAllSixtyFourBits(int $number): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->intParamToString($number),
            fn () => 'n=' . $number
        );
    }

    /**
     * @dataProvider unsignedProvider
     */
    public function testULongParameterKeepsAllSixtyFourBits(int $number): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->ulongParamToString($number),
            fn () => 'n=' . $number
        );
    }

    /**
     * @dataProvider sixtyFourBitProvider
     */
    public function testLongSurvivesParameterLocalAndReturn(int $number): void
    {
        $this->assertSame($number, $this->test->longRoundTrip($number));
        $this->assertSame($number, $this->test->longThroughLocal($number));
    }

    /**
     * @dataProvider unsignedProvider
     */
    public function testULongSurvivesParameterAndReturn(int $number): void
    {
        $this->assertSame($number, $this->test->ulongRoundTrip($number));
    }

    /**
     * `zephir_get_intval()` is the funnel shared by every cast, `substr()`
     * offset and internal-method coercion. Its `_ex` helper returned a C
     * `long`, so Zephir `int` -- which the issue calls safe -- truncated too.
     */
    public static function castProvider(): array
    {
        return [
            'int64 max'         => [PHP_INT_MAX],
            'int64 min'         => [PHP_INT_MIN],
            'above int32'       => [4294967296],
            'numeric string'    => ['9223372036854775807'],
            'negative string'   => ['-9223372036854775808'],
            'leading spaces'    => ['   4294967296'],
            'trailing garbage'  => ['4294967296abc'],
            'float in range'    => [4294967296.0],
            'true'              => [true],
            'false'             => [false],
            'null'              => [null],
            'empty array'       => [[]],
            'filled array'      => [[1, 2]],
        ];
    }

    /**
     * @dataProvider castProvider
     */
    public function testCastToIntMatchesPhp(mixed $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->castToInt($value),
            fn () => (int) $value
        );
    }

    /**
     * @dataProvider castProvider
     */
    public function testCastToLongMatchesPhp(mixed $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->castToLong($value),
            fn () => (int) $value
        );
    }

    public function testArithmeticOnTwoLongsStaysSixtyFourBit(): void
    {
        $this->assertSame(PHP_INT_MAX, $this->test->longAdd(PHP_INT_MAX - 1, 1));
        $this->assertSame(4294967297, $this->test->longAdd(4294967296, 1));
    }

    /**
     * @dataProvider sixtyFourBitProvider
     */
    public function testLongArrayKeyMatchesPhp(int $key): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->longArrayKey($key, 'v'),
            fn () => [$key => 'v']
        );

        $source = [$key => 'v'];

        $this->assertMatchesPhp(
            fn () => $this->test->longArrayFetch($source, $key),
            fn () => $source[$key]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->longArrayIsset($source, $key),
            fn () => isset($source[$key])
        );
    }

    /**
     * @dataProvider sixtyFourBitProvider
     */
    public function testComparisonAgainstALongMatchesPhp(int $limit): void
    {
        foreach ([0, 4294967296, PHP_INT_MAX, PHP_INT_MIN, '4294967296'] as $value) {
            $this->assertMatchesPhp(
                fn () => $this->test->varLessThanLong($value, $limit),
                fn () => $value < $limit
            );
            $this->assertMatchesPhp(
                fn () => $this->test->varGreaterThanLong($value, $limit),
                fn () => $value > $limit
            );
        }
    }

    public function testSubstrWithLongOffsetMatchesPhp(): void
    {
        $source = 'abcdef';

        foreach ([[0, 3], [2, 2], [-3, 2], [1, -1]] as [$from, $length]) {
            $this->assertMatchesPhp(
                fn () => $this->test->substrWithLongOffset($source, $from, $length),
                fn () => substr($source, $from, $length)
            );
        }
    }

    /**
     * Limits of 0, 1 and negatives are deliberately absent: `zephir_fast_explode()`
     * hands every limit straight to `php_explode()`, while PHP's own explode()
     * routes `limit <= 1` and negative limits elsewhere. That is a separate
     * defect from the operand width this issue is about.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2674
     */
    public function testExplodeWithLongLimitMatchesPhp(): void
    {
        $source = 'a,b,c,d';

        foreach ([2, 3, 4294967296, PHP_INT_MAX] as $limit) {
            $this->assertMatchesPhp(
                fn () => $this->test->explodeWithLongLimit($source, $limit),
                fn () => explode(',', $source, $limit)
            );
        }
    }

    /**
     * A multi-dimensional key travels through a variadic slot that
     * `zephir_array_update_multi()` reads back as `zend_long`. Passing it as a
     * C `int`, which every integer literal is, leaves the upper half of the
     * slot undefined; Windows returned keys such as 140733193388033 for 1.
     * `uint`/`ulong` offsets were passed as `&i` and used the pointer value.
     *
     * @dataProvider sixtyFourBitProvider
     */
    public function testMultiDimensionalKeysMatchPhp(int $key): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->multiIntKey($key),
            fn () => [$key => [2 => 'v']]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->multiLongKey($key),
            fn () => [$key => [2 => 'v']]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->multiMixedKeys($key),
            fn () => ['k' => [$key => 'v']]
        );
    }

    /**
     * @dataProvider unsignedProvider
     */
    public function testUnsignedMultiDimensionalKeysMatchPhp(int $key): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->multiUintKey($key),
            fn () => [$key => [2 => 'v']]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->multiUlongKey($key),
            fn () => [$key => [2 => 'v']]
        );
    }

    public function testLiteralMultiDimensionalKeysMatchPhp(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->multiLiteralKeys(),
            fn () => [0 => [1 => 'v']]
        );
        $this->assertMatchesPhp(
            fn () => $this->test->multiDeepKeys(),
            fn () => [0 => [1 => [2 => [3 => [4 => [5 => [6 => [7 => [8 => 'v']]]]]]]]]
        );
    }

    /**
     * Every shape of `%` the compiler can emit, against the identical PHP
     * expression. PHP's `%` always yields an `int`, so routing the result
     * through a C `double` -- which Zephir did -- loses every value above
     * 2^53 on every platform, not just Windows.
     */
    public function testModuloMatchesPhp(): void
    {
        foreach ($this->moduloOperands() as [$a, $b]) {
            $this->assertMatchesPhp(
                fn () => $this->test->modLongLong($a, $b),
                fn () => $a % $b
            );
            $this->assertMatchesPhp(
                fn () => $this->test->modVarLong($a, $b),
                fn () => $a % $b
            );
            $this->assertMatchesPhp(
                fn () => $this->test->modLongVar($a, $b),
                fn () => $a % $b
            );
        }
    }

    public function testModuloWithADoubleOperandMatchesPhp(): void
    {
        foreach ([7, -7, 0, PHP_INT_MAX, PHP_INT_MIN] as $a) {
            $this->assertMatchesPhp(
                fn () => $this->test->modLongDouble($a),
                fn () => $a % 2.5
            );
            $this->assertMatchesPhp(
                fn () => $this->test->modVarDouble($a),
                fn () => $a % 2.5
            );
        }

        foreach ([7.0, -7.5, 0.0, 2.5] as $a) {
            $this->assertMatchesPhp(
                fn () => $this->test->modDoubleLong($a, 3),
                fn () => $a % 3
            );
            $this->assertMatchesPhp(
                fn () => $this->test->modDoubleDouble($a),
                fn () => $a % 2.5
            );
            $this->assertMatchesPhp(
                fn () => $this->test->modDoubleVar($a, 3),
                fn () => $a % 3
            );
            $this->assertMatchesPhp(
                fn () => $this->test->modLiteralDoubleByDouble($a),
                fn () => 2.5 % $a
            );
        }
    }

    /**
     * php-src special-cases a `-1` divisor in `mod_function()` because
     * `PHP_INT_MIN % -1` overflows and raises SIGFPE on x86.
     */
    public function testModuloByMinusOneDoesNotCrash(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->modLongLong(PHP_INT_MIN, -1),
            fn () => PHP_INT_MIN % -1
        );
        $this->assertMatchesPhp(
            fn () => $this->test->modVarLong(PHP_INT_MIN, -1),
            fn () => PHP_INT_MIN % -1
        );
    }

    /**
     * PHP 8 throws DivisionByZeroError; Zephir raised an E_WARNING and
     * returned 0.
     */
    public function testModuloByZeroThrowsAsPhpDoes(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->modLongLong(7, 0),
            fn () => 7 % 0
        );
        $this->assertMatchesPhp(
            fn () => $this->test->modVarLong(7, 0),
            fn () => 7 % 0
        );
        $this->assertMatchesPhp(
            fn () => $this->test->modLongVar(7, 0),
            fn () => 7 % 0
        );
        $this->assertMatchesPhp(
            fn () => $this->test->modDoubleLong(7.0, 0),
            fn () => 7.0 % 0
        );
        // (int) 0.5 is 0, so PHP throws here too.
        $this->assertMatchesPhp(
            fn () => $this->test->modLiteralDoubleByDouble(0.5),
            fn () => 2.5 % 0.5
        );
    }

    public function testDivisionByZeroThrowsAsPhpDoes(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->divLongLong(7, 0),
            fn () => 7 / 0
        );
        $this->assertMatchesPhp(
            fn () => $this->test->divVarLong(7, 0),
            fn () => 7 / 0
        );
        $this->assertMatchesPhp(
            fn () => $this->test->divLongVar(7, 0),
            fn () => 7 / 0
        );
        $this->assertMatchesPhp(
            fn () => $this->test->divDoubleLong(7.0, 0),
            fn () => 7.0 / 0
        );
        $this->assertMatchesPhp(
            fn () => $this->test->divDoubleVar(7.0, 0),
            fn () => 7.0 / 0
        );
        $this->assertMatchesPhp(
            fn () => $this->test->divLiteralDoubleByDouble(0.0),
            fn () => 2.5 / 0.0
        );
    }

    /**
     * A non-exact division is a float in PHP too, so the whole shape set can
     * be compared against PHP directly.
     */
    public function testNonExactDivisionMatchesPhp(): void
    {
        $this->assertMatchesPhp(fn () => $this->test->divLongLong(7, 2), fn () => 7 / 2);
        $this->assertMatchesPhp(fn () => $this->test->divVarLong(7, 2), fn () => 7 / 2);
        $this->assertMatchesPhp(fn () => $this->test->divLongVar(7, 2), fn () => 7 / 2);
        $this->assertMatchesPhp(fn () => $this->test->divLongDouble(7), fn () => 7 / 2.5);
        $this->assertMatchesPhp(fn () => $this->test->divVarDouble(7), fn () => 7 / 2.5);
        $this->assertMatchesPhp(fn () => $this->test->divDoubleLong(7.0, 2), fn () => 7.0 / 2);
        $this->assertMatchesPhp(fn () => $this->test->divDoubleDouble(7.0), fn () => 7.0 / 2.5);
        $this->assertMatchesPhp(fn () => $this->test->divDoubleVar(7.0, 2), fn () => 7.0 / 2);
        $this->assertMatchesPhp(fn () => $this->test->divLiteralDoubleByDouble(2.0), fn () => 2.5 / 2.0);
    }

    /**
     * Known divergence, pinned so that closing it is a deliberate act: PHP's
     * `/` narrows an exact result to `int`, Zephir's helpers always return a
     * C double.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2675
     */
    public function testExactDivisionStillReturnsFloat(): void
    {
        $this->assertSame(2, 4 / 2);
        $this->assertSame(2.0, $this->test->divLongLong(4, 2));
    }

    /**
     * @return list<array{0: int, 1: int}>
     */
    private function moduloOperands(): array
    {
        return [
            [7, 3],
            [-7, 3],
            [7, -3],
            [PHP_INT_MAX, 10],
            [PHP_INT_MIN, 10],
            [PHP_INT_MAX, PHP_INT_MIN],
            [4294967296, 4294967295],
            [PHP_INT_MAX, PHP_INT_MAX],
        ];
    }
}
