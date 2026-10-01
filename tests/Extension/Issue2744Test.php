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
use Stub\Issue2744;

/**
 * PHP's `+`, `-` and `*` keep a float operand's fraction, overflow an int
 * result to float, and throw TypeError for an operand they cannot treat as a
 * number. A zval combined with a native number must do the same. Every
 * assertion compares the extension against the same PHP expression.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2744
 */
final class Issue2744Test extends TestCase
{
    use AssertsPhpParity;

    private const OPERATORS = ['add' => '+', 'sub' => '-', 'mul' => '*'];

    private Issue2744 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2744();
    }

    /**
     * Each shape maps the zval value to the extension arguments and to the
     * PHP operands: [left, right].
     */
    private static function shapes(): array
    {
        return [
            'VarLiteral'       => [fn ($a) => [$a], fn ($a) => [$a, 3]],
            'LiteralVar'       => [fn ($a) => [$a], fn ($a) => [3, $a]],
            'VarDoubleLiteral' => [fn ($a) => [$a], fn ($a) => [$a, 1.5]],
            'DoubleLiteralVar' => [fn ($a) => [$a], fn ($a) => [1.5, $a]],
            'LongVar'          => [fn ($a) => [2, $a], fn ($a) => [2, $a]],
            'VarLong'          => [fn ($a) => [$a, 2], fn ($a) => [$a, 2]],
            'DoubleVar'        => [fn ($a) => [0.5, $a], fn ($a) => [0.5, $a]],
            'VarDouble'        => [fn ($a) => [$a, 0.5], fn ($a) => [$a, 0.5]],
            'BoolVar'          => [fn ($a) => [true, $a], fn ($a) => [true, $a]],
            'VarBool'          => [fn ($a) => [$a, true], fn ($a) => [$a, true]],
            'CharVar'          => [fn ($a) => [$a], fn ($a) => [97, $a]],
        ];
    }

    private static function values(): array
    {
        return [
            'float'                => 2.5,
            'negative float'       => -0.5,
            'numeric float string' => '1.5',
            'numeric int string'   => '4',
            'int'                  => 7,
            'max int'              => PHP_INT_MAX,
            'min int'              => PHP_INT_MIN,
            'null'                 => null,
            'true'                 => true,
            'false'                => false,
            'leading-numeric'      => '5abc',
            'non-numeric string'   => 'abc',
            'array'                => [1],
            'object'               => new stdClass(),
        ];
    }

    public static function zvalWithNumberProvider(): iterable
    {
        foreach (self::shapes() as $shape => [$arguments, $operands]) {
            foreach (self::OPERATORS as $prefix => $operator) {
                foreach (self::values() as $label => $value) {
                    yield $prefix . $shape . ' ' . $label => [
                        $prefix . $shape,
                        $operator,
                        $arguments($value),
                        $operands($value),
                    ];
                }
            }
        }
    }

    /**
     * @dataProvider zvalWithNumberProvider
     */
    public function testZvalWithNativeNumberMatchesPhp(
        string $method,
        string $operator,
        array $arguments,
        array $operands
    ): void {
        $this->assertMatchesPhp(
            fn () => $this->test->{$method}(...$arguments),
            fn () => self::apply($operator, ...$operands)
        );
    }

    public static function arrayProvider(): iterable
    {
        foreach (self::OPERATORS as $prefix => $operator) {
            yield $prefix . ' empty'     => [$prefix . 'ArrayLiteral', $operator, []];
            yield $prefix . ' non-empty' => [$prefix . 'ArrayLiteral', $operator, [1, 2]];
        }
    }

    /**
     * @dataProvider arrayProvider
     */
    public function testArrayWithNumberThrowsLikePhp(string $method, string $operator, array $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->{$method}($value),
            fn () => self::apply($operator, $value, 3)
        );
    }

    public static function assignmentProvider(): array
    {
        return [
            'float'              => [2.5],
            'string'             => ['1.5'],
            'int'                => [7],
            'max int'            => [PHP_INT_MAX],
            'array'              => [[1]],
            'non-numeric string' => ['abc'],
        ];
    }

    /**
     * @dataProvider assignmentProvider
     */
    public function testAssignedToVarMatchesPhp(mixed $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->assignedToVar($value),
            fn () => $value + 1
        );
    }

    /**
     * A `long` local has no PHP equivalent, so an `(int)` cast stands in.
     * PHP 8.5 warns when that cast meets a float beyond the int range; a
     * typed local is not a cast, so the overflowing row is left out.
     */
    public static function longTargetProvider(): array
    {
        $rows = self::assignmentProvider();
        unset($rows['max int']);

        return $rows;
    }

    /**
     * @dataProvider longTargetProvider
     */
    public function testAssignedToLongMatchesAnIntCast(mixed $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->assignedToLong($value),
            fn () => (int) ($value + 1)
        );
    }

    /**
     * @dataProvider assignmentProvider
     */
    public function testAssignedToDoubleMatchesAFloatCast(mixed $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->assignedToDouble($value),
            fn () => (float) ($value + 1)
        );
    }

    /**
     * The use compiles while `a` has only been assigned an int, but the
     * second iteration reads the float assigned later in the loop body.
     */
    public function testLoopCarriedFloatIsNotTruncated(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->loopCarried(),
            static function (): array {
                $a       = 1;
                $results = [];
                foreach (range(1, 2) as $ignored) {
                    $results[] = $a * 2;
                    $a         = 0.5;
                }

                return $results;
            }
        );
    }

    /**
     * @dataProvider assignmentProvider
     */
    public function testCompoundAssignmentsMatchPhp(mixed $value): void
    {
        $this->assertMatchesPhp(fn () => $this->test->addAssign($value), fn () => $value + 1);
        $this->assertMatchesPhp(fn () => $this->test->subAssign($value), fn () => $value - 1);
        $this->assertMatchesPhp(fn () => $this->test->mulAssign($value), fn () => $value * 2);
    }

    public static function incrementProvider(): array
    {
        return [
            'float'   => [2.5],
            'string'  => ['1.5'],
            'int'     => [7],
            'max int' => [PHP_INT_MAX],
            'min int' => [PHP_INT_MIN],
            'null'    => [null],
        ];
    }

    /**
     * @dataProvider incrementProvider
     */
    public function testIncrementAndDecrementMatchPhp(mixed $value): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->increment($value),
            static function () use ($value) {
                ++$value;

                return $value;
            }
        );
        $this->assertMatchesPhp(
            fn () => $this->test->decrement($value),
            static function () use ($value) {
                --$value;

                return $value;
            }
        );
    }

    private static function apply(string $operator, mixed $left, mixed $right): mixed
    {
        return match ($operator) {
            '+' => $left + $right,
            '-' => $left - $right,
            '*' => $left * $right,
        };
    }
}
