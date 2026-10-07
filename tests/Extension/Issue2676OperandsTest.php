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
use Stub\Issue2676Operands;

/**
 * Every arithmetic operator with a string, array or null operand, which PHP
 * coerces, warns about or rejects with TypeError, and `+ - *` with a bool
 * operand, which PHP reads as the integer 0 or 1. Every assertion compares
 * the extension against the same PHP expression.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2676
 * @issue https://github.com/zephir-lang/zephir/issues/2677
 */
final class Issue2676OperandsTest extends TestCase
{
    use AssertsPhpParity;

    private const OPERATORS = [
        'add' => '+',
        'sub' => '-',
        'mul' => '*',
        'div' => '/',
        'mod' => '%',
    ];

    private const STRINGS = ['10', ' 7', '1e3', '7.5', '5 apples', 'abc', ''];

    private const LONGS = [3, -1, 0];

    private const DOUBLES = [2.5, 0.0];

    private const BOOLS = [true, false];

    private const MIXED = [3, 2.5, 0, '4', 'x', null, true, [1]];

    private Issue2676Operands $test;

    protected function setUp(): void
    {
        $this->test = new Issue2676Operands();
    }

    public static function operatorProvider(): array
    {
        $cases = [];
        foreach (self::OPERATORS as $name => $operator) {
            $cases[$name] = [$name, $operator];
        }

        return $cases;
    }

    public static function nativeOperatorProvider(): array
    {
        return array_intersect_key(self::operatorProvider(), array_flip(['add', 'sub', 'mul']));
    }

    /**
     * @dataProvider operatorProvider
     */
    public function testStringLocal(string $name, string $operator): void
    {
        foreach (self::STRINGS as $a) {
            foreach (self::LONGS as $b) {
                $this->assertOperation($name . 'StringLong', $operator, $a, $b);
                $this->assertOperation($name . 'LongString', $operator, $b, $a);
                $this->assertOperation($name . 'InferredLocal', $operator, $a, $b);
            }
            foreach (self::DOUBLES as $b) {
                $this->assertOperation($name . 'StringDouble', $operator, $a, $b);
            }
            foreach (self::BOOLS as $b) {
                $this->assertOperation($name . 'StringBool', $operator, $a, $b);
            }
            foreach (self::MIXED as $b) {
                $this->assertOperation($name . 'StringVar', $operator, $a, $b);
            }
            foreach (self::STRINGS as $b) {
                $this->assertOperation($name . 'StringString', $operator, $a, $b);
            }
        }
    }

    /**
     * @dataProvider operatorProvider
     */
    public function testArrayLocal(string $name, string $operator): void
    {
        foreach ([[1, 2], []] as $a) {
            foreach (self::LONGS as $b) {
                $this->assertOperation($name . 'ArrayLong', $operator, $a, $b);
            }
            foreach (self::MIXED as $b) {
                $this->assertOperation($name . 'ArrayVar', $operator, $a, $b);
            }
        }
    }

    /**
     * @dataProvider operatorProvider
     */
    public function testLiterals(string $name, string $operator): void
    {
        foreach (self::MIXED as $b) {
            $this->assertLiteral($name . 'LiteralString', $operator, '10', $b);
            $this->assertLiteral($name . 'LiteralAbc', $operator, 'abc', $b);
            $this->assertLiteral($name . 'NullVar', $operator, null, $b);
            $this->assertLiteral($name . 'LiteralArray', $operator, [1], $b);
        }
        foreach (self::LONGS as $b) {
            $this->assertLiteral($name . 'LiteralLong', $operator, '10', $b);
            $this->assertLiteral($name . 'NullLong', $operator, null, $b);
        }
    }

    /**
     * @dataProvider operatorProvider
     */
    public function testCompoundAssignment(string $name, string $operator): void
    {
        foreach (self::MIXED as $a) {
            $this->assertMatchesPhp(
                fn () => $this->test->{$name . 'AssignVarLiteralString'}($a),
                fn () => self::php($operator, $a, '10'),
                $name . 'AssignVarLiteralString(' . var_export($a, true) . ')'
            );
            $this->assertMatchesPhp(
                fn () => $this->test->{$name . 'AssignVarNull'}($a),
                fn () => self::php($operator, $a, null),
                $name . 'AssignVarNull(' . var_export($a, true) . ')'
            );
            foreach (self::STRINGS as $b) {
                $this->assertOperation($name . 'AssignVarString', $operator, $a, $b);
            }
            foreach (['LiteralString' => '10', 'Null' => null] as $shape => $b) {
                $this->assertMatchesPhp(
                    fn () => $this->test->{$name . 'AssignProperty' . $shape}((object) ['value' => $a]),
                    fn () => self::php($operator, $a, $b),
                    $name . 'AssignProperty' . $shape . '(' . var_export($a, true) . ')'
                );
            }
        }
    }

    /**
     * @dataProvider nativeOperatorProvider
     */
    public function testBoolOperands(string $name, string $operator): void
    {
        foreach (self::BOOLS as $a) {
            foreach ([7, -2] as $b) {
                $this->assertOperation($name . 'BoolLong', $operator, $a, $b);
            }
            foreach (self::BOOLS as $b) {
                $this->assertOperation($name . 'BoolBool', $operator, $a, $b);
            }
            foreach ([2.5, -0.5] as $b) {
                $this->assertOperation($name . 'DoubleBool', $operator, $b, $a);
                $this->assertOperation($name . 'BoolDouble', $operator, $a, $b);
            }
        }
        foreach ([7, -2] as $b) {
            $this->assertLiteral($name . 'TrueLong', $operator, true, $b);
            $this->assertMatchesPhp(
                fn () => $this->test->{$name . 'LongTrue'}($b),
                fn () => self::php($operator, $b, true)
            );
        }
        $this->assertMatchesPhp(
            fn () => $this->test->{$name . 'DoubleTrue'}(2.5),
            fn () => self::php($operator, 2.5, true)
        );
    }

    /**
     * @dataProvider nativeOperatorProvider
     */
    public function testLongWithDoubleLiteral(string $name, string $operator): void
    {
        foreach ([7, -3, 0] as $a) {
            $this->assertMatchesPhp(
                fn () => $this->test->{$name . 'LongLiteralDouble'}($a),
                fn () => self::php($operator, $a, 1.5)
            );
        }
    }

    private function assertOperation(string $method, string $operator, mixed $a, mixed $b): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->{$method}($a, $b),
            fn () => self::php($operator, $a, $b),
            $method . '(' . var_export($a, true) . ', ' . var_export($b, true) . ')'
        );
    }

    /**
     * The left operand is a literal in the Zephir method.
     */
    private function assertLiteral(string $method, string $operator, mixed $a, mixed $b): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->{$method}($b),
            fn () => self::php($operator, $a, $b),
            $method . '(' . var_export($b, true) . ')'
        );
    }

    private static function php(string $operator, mixed $a, mixed $b): mixed
    {
        return match ($operator) {
            '+' => $a + $b,
            '-' => $a - $b,
            '*' => $a * $b,
            '/' => $a / $b,
            '%' => $a % $b,
        };
    }
}
