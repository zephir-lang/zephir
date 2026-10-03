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

use ArrayObject;
use Issue2691Overloaded;
use Issue2747Holder;
use Issue2747Order;
use Issue2747ReadonlyHolder;
use Issue2747TypedHolder;
use Issue2747TypedRefHolder;
use PHPUnit\Framework\TestCase;
use stdClass;
use Stub\Issue2747;

use const PHP_INT_MAX;
use const PHP_VERSION_ID;

/**
 * `a[k] OP= v` reads the element, applies the operator and writes the result
 * back: PHP's ZEND_ASSIGN_DIM_OP. The containers along the way are fetched
 * read-write, so a missing key warns before it reads as null, null and false
 * become arrays, an ArrayAccess object goes through offsetGet() and
 * offsetSet(), and a string or a scalar throws. Every assertion compares the
 * extension against the same PHP statement.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2747
 */
final class Issue2747Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2747 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2747();
    }

    /**
     * @return array<string, callable(mixed, mixed, mixed): mixed>
     */
    private static function operators(): array
    {
        return [
            'add'        => static function ($a, $k, $v) {
                $a[$k] += $v;
                return $a;
            },
            'sub'        => static function ($a, $k, $v) {
                $a[$k] -= $v;
                return $a;
            },
            'mul'        => static function ($a, $k, $v) {
                $a[$k] *= $v;
                return $a;
            },
            'div'        => static function ($a, $k, $v) {
                $a[$k] /= $v;
                return $a;
            },
            'mod'        => static function ($a, $k, $v) {
                $a[$k] %= $v;
                return $a;
            },
            'concat'     => static function ($a, $k, $v) {
                $a[$k] .= $v;
                return $a;
            },
            'bitwiseAnd' => static function ($a, $k, $v) {
                $a[$k] &= $v;
                return $a;
            },
            'bitwiseOr'  => static function ($a, $k, $v) {
                $a[$k] |= $v;
                return $a;
            },
            'bitwiseXor' => static function ($a, $k, $v) {
                $a[$k] ^= $v;
                return $a;
            },
            'shiftLeft'  => static function ($a, $k, $v) {
                $a[$k] <<= $v;
                return $a;
            },
            'shiftRight' => static function ($a, $k, $v) {
                $a[$k] >>= $v;
                return $a;
            },
        ];
    }

    /**
     * Each case builds its operands afresh, so the extension and PHP never
     * share an object or a reference.
     */
    public static function operandProvider(): array
    {
        return [
            'int'                 => [static fn () => [['k' => 10], 'k', 2]],
            'negative'            => [static fn () => [['k' => -7], 'k', 3]],
            'float'               => [static fn () => [['k' => 7.5], 'k', 2]],
            'numeric strings'     => [static fn () => [['k' => '12'], 'k', '3']],
            'non-numeric strings' => [static fn () => [['k' => 'ab'], 'k', 'x']],
            'null operand'        => [static fn () => [['k' => 7], 'k', null]],
            'zero divisor'        => [static fn () => [['k' => 7], 'k', 0]],
            'array element'       => [static fn () => [['k' => [1]], 'k', 1]],
            'array operands'      => [static fn () => [['k' => [1]], 'k', [5, 6]]],
            'missing string key'  => [static fn () => [['k' => 10], 'm', 2]],
            'missing int key'     => [static fn () => [[1 => 10], 7, 2]],
            'int key'             => [static fn () => [[1 => 10], 1, 2]],
            'numeric string key'  => [static fn () => [[3 => 10], '3', 2]],
            'float key'           => [static fn () => [[1 => 10], 1.5, 2]],
            'bool key'            => [static fn () => [[1 => 10], true, 2]],
            'null key'            => [static fn () => [['' => 10], null, 2]],
            'array key'           => [static fn () => [[1 => 10], [], 2]],
            'null container'      => [static fn () => [null, 'k', 2]],
            'false container'     => [static fn () => [false, 'k', 2]],
            'string container'    => [static fn () => ['abc', 0, 'x']],
            'string bad offset'   => [static fn () => ['abc', 'x', 'y']],
            'int container'       => [static fn () => [5, 'k', 2]],
            'true container'      => [static fn () => [true, 'k', 2]],
            'stdClass container'  => [static fn () => [new stdClass(), 'k', 2]],
            'ArrayObject'         => [static fn () => [new ArrayObject(['k' => 10]), 'k', 2]],
            'ArrayObject missing' => [static fn () => [new ArrayObject([]), 'k', 2]],
            'reference element'   => [static function () {
                $x = 10;

                return [['k' => &$x], 'k', 2];
            }],
        ];
    }

    /**
     * @dataProvider operandProvider
     */
    public function testEveryOperator(callable $operands): void
    {
        foreach (self::operators() as $method => $php) {
            $this->assertMatchesPhp(
                fn () => $this->test->{$method}(...$operands()),
                static fn () => $php(...$operands()),
            );
        }
    }

    public function testTypedReferenceElementIsVerified(): void
    {
        $operands = static function () {
            $holder = new Issue2747TypedRefHolder();

            return [['k' => &$holder->i, 'holder' => $holder], 'k', 'x'];
        };

        $this->assertMatchesPhp(
            fn () => $this->test->concat(...$operands()),
            static function () use ($operands) {
                [$a, $k, $v] = $operands();
                $a[$k] .= $v;

                return $a;
            },
        );
    }

    /**
     * The filed reproduction, and the other key and value kinds the compiler
     * emits differently: literal keys, native integer and string keys, and
     * native right-hand sides.
     */
    public function testKeyAndValueKinds(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->addStringLiteralKey(['k' => 10], 2),
            static function () {
                $a = ['k' => 10];
                $a['k'] += 2;
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->addStringLiteralKey([], 2),
            static function () {
                $a = [];
                $a['k'] += 2;
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->addIntLiteralKey([1 => 10], 2),
            static function () {
                $a = [1 => 10];
                $a[1] += 2;
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->addIntLiteralKey([], 2),
            static function () {
                $a = [];
                $a[1] += 2;
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->addLongKey([3 => 10], 3, 2),
            static function () {
                $a = [3 => 10];
                $a[3] += 2;
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->addStringKey(['3' => 10, 'k' => 1], '3', 2),
            static function () {
                $a = ['3' => 10, 'k' => 1];
                $a['3'] += 2;
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->addIntLiteral(['k' => 10]),
            static function () {
                $a = ['k' => 10];
                $a['k'] += 2;
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->concatStringLiteral(['k' => 'ab']),
            static function () {
                $a = ['k' => 'ab'];
                $a['k'] .= 'x';
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->addLongValue(['k' => 10], 'k', 2),
            static function () {
                $a = ['k' => 10];
                $a['k'] += 2;
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->addLongValue(['k' => PHP_INT_MAX], 'k', 1),
            static function () {
                $a = ['k' => PHP_INT_MAX];
                $a['k'] += 1;
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->mulDoubleValue(['k' => 3], 'k', 1.5),
            static function () {
                $a = ['k' => 3];
                $a['k'] *= 1.5;
                return $a;
            },
        );
    }

    /**
     * The right-hand side is read from the very element it modifies; an
     * in-place `.=` must not free the operand under itself.
     */
    public function testOperandReadFromTheSameElement(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->concatOwnElement(['k' => str_repeat('ab', 8)]),
            static function () {
                $a = ['k' => str_repeat('ab', 8)];
                $a['k'] .= $a['k'];
                return $a;
            },
        );
    }

    public function testCopyIsSeparated(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->addToCopy(['k' => 10], 'k', 2),
            static function () {
                $a = ['k' => 10];
                $b = $a;
                $b['k'] += 2;
                return [$a, $b];
            },
        );
    }

    public static function nestedProvider(): array
    {
        return [
            'existing'           => [static fn () => [[1 => [2 => 3]], 1, 2, 3]],
            'both missing'       => [static fn () => [[], 5, 6, 1]],
            'inner missing'      => [static fn () => [[5 => []], 5, 6, 1]],
            'null container'     => [static fn () => [null, 5, 6, 1]],
            'scalar inside'      => [static fn () => [[1 => 5], 1, 2, 1]],
            'string inside'      => [static fn () => [[1 => 'abc'], 1, 0, 'x']],
            'string container'   => [static fn () => ['abc', 0, 1, 'x']],
            'ArrayObject'        => [static fn () => [new ArrayObject(['k' => ['j' => 1]]), 'k', 'j', 1]],
            'ArrayObject inside' => [static fn () => [['k' => new ArrayObject(['j' => 1])], 'k', 'j', 1]],
            'stdClass'           => [static fn () => [new stdClass(), 'k', 'j', 1]],
        ];
    }

    /**
     * @dataProvider nestedProvider
     */
    public function testNestedOffsets(callable $operands): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->addNested(...$operands()),
            static function () use ($operands) {
                [$a, $i, $j, $v] = $operands();
                $a[$i][$j] += $v;

                return $a;
            },
        );
    }

    public function testNestedLiteralOffsets(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->concatNestedLiteral([1 => ['x' => 'a']], 'b'),
            static function () {
                $a = [1 => ['x' => 'a']];
                $a[1]['x'] .= 'b';
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->concatNestedLiteral([], 'b'),
            static function () {
                $a = [];
                $a[1]['x'] .= 'b';
                return $a;
            },
        );
    }

    public static function appendProvider(): array
    {
        return [
            'empty'          => [static fn () => [[], 2]],
            'list'           => [static fn () => [[1, 2], 2]],
            'null container' => [static fn () => [null, 2]],
            'occupied'       => [static fn () => [[PHP_INT_MAX => 1], 2]],
            'string'         => [static fn () => ['abc', 2]],
            'int'            => [static fn () => [5, 2]],
            'ArrayObject'    => [static fn () => [new ArrayObject([1]), 2]],
        ];
    }

    /**
     * PHP appends a null and applies the operator to it: `a[] -= 2` stores -2.
     *
     * @dataProvider appendProvider
     */
    public function testAppend(callable $operands): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->subAppend(...$operands()),
            static function () use ($operands) {
                [$a, $v] = $operands();
                $a[] -= $v;

                return $a;
            },
        );
    }

    public function testNestedAppend(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->concatNestedAppend(['k' => ['a']], 'k', 'b'),
            static function () {
                $a = ['k' => ['a']];
                $a['k'][] .= 'b';
                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->concatNestedAppend([], 'k', 'b'),
            static function () {
                $a = [];
                $a['k'][] .= 'b';
                return $a;
            },
        );
    }

    public static function propertyProvider(): array
    {
        return [
            'existing key' => [static fn () => [['k' => 10], 'k', 2]],
            'missing key'  => [static fn () => [['k' => 10], 'm', 2]],
            'null'         => [static fn () => [null, 'k', 2]],
            'string'       => [static fn () => ['abc', 0, 'x']],
            'int'          => [static fn () => [5, 'k', 2]],
            'ArrayObject'  => [static fn () => [new ArrayObject(['k' => 10]), 'k', 2]],
        ];
    }

    /**
     * @dataProvider propertyProvider
     */
    public function testThisProperty(callable $operands): void
    {
        $this->assertMatchesPhp(
            function () use ($operands) {
                [$p, $k, $v] = $operands();
                $this->test->setProperties($p);

                return $this->test->addThis($k, $v);
            },
            static function () use ($operands) {
                [$p, $k, $v] = $operands();
                $p[$k] += $v;

                return $p;
            },
        );
    }

    /**
     * @dataProvider propertyProvider
     */
    public function testStaticProperty(callable $operands): void
    {
        $this->assertMatchesPhp(
            function () use ($operands) {
                [$p, $k, $v] = $operands();
                $this->test->setProperties($p);

                return $this->test->addStatic($k, $v);
            },
            static function () use ($operands) {
                [$p, $k, $v] = $operands();
                $p[$k] += $v;

                return $p;
            },
        );
    }

    public function testPropertyNestedAndAppend(): void
    {
        $holder = function (string $method, array $p, ...$args) {
            $this->test->setProperties($p);

            return $this->test->{$method}(...$args);
        };

        $this->assertMatchesPhp(
            fn () => $holder('addThisNested', ['k' => ['j' => 1]], 'k', 'j', 2),
            static function () {
                $p = ['k' => ['j' => 1]];
                $p['k']['j'] += 2;
                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $holder('addThisNested', [], 'k', 'j', 2),
            static function () {
                $p = [];
                $p['k']['j'] += 2;
                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $holder('subThisAppend', [1], 2),
            static function () {
                $p = [1];
                $p[] -= 2;
                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $holder('concatThisNestedAppend', ['k' => ['a']], 'k', 'b'),
            static function () {
                $p = ['k' => ['a']];
                $p['k'][] .= 'b';
                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $holder('addStaticNested', ['k' => ['j' => 1]], 'k', 'j', 2),
            static function () {
                $p = ['k' => ['j' => 1]];
                $p['k']['j'] += 2;
                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $holder('subStaticAppend', [1], 2),
            static function () {
                $p = [1];
                $p[] -= 2;
                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $holder('concatStaticNestedAppend', [], 'k', 'b'),
            static function () {
                $p = [];
                $p['k'][] .= 'b';
                return $p;
            },
        );
    }

    public static function objectProvider(): array
    {
        $cases = [
            'plain'              => [static fn () => new Issue2747Holder(['k' => 10])],
            'plain missing key'  => [static fn () => new Issue2747Holder([])],
            'plain null'         => [static fn () => new Issue2747Holder(null)],
            'uninitialized'      => [static fn () => new Issue2747TypedHolder()],
            'overloaded'         => [static fn () => new Issue2691Overloaded()],
            'undefined property' => [static fn () => new stdClass()],
            'int'                => [static fn () => 5],
            'null'               => [static fn () => null],
        ];

        if (PHP_VERSION_ID >= 80100) {
            $cases['readonly'] = [static fn () => new Issue2747ReadonlyHolder()];
        }

        return $cases;
    }

    /**
     * The property is fetched read-write: an uninitialized typed property
     * throws, an undefined one warns, a readonly one cannot be modified.
     *
     * @dataProvider objectProvider
     */
    public function testObjectProperty(callable $object): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->addObject($object(), 'k', 2),
            static function () use ($object) {
                $o = $object();
                $o->p['k'] += 2;

                return $o;
            },
        );
    }

    public static function orderProvider(): array
    {
        return [
            'local'                      => ['orderLocal'],
            'local nested'               => ['orderLocalNested'],
            'local append'               => ['orderLocalAppend'],
            'compound'                   => ['orderCompound'],
            'this property'              => ['orderThis'],
            'this property nested'       => ['orderThisNested'],
            'static property'            => ['orderStatic'],
            'static property append'     => ['orderStaticAppend'],
            'property read as index'     => ['orderPropertyIndex'],
            'property read as value'     => ['orderPropertyValue'],
            'computed index'             => ['orderComputedIndex'],
            'string offset'              => ['orderStringOffset'],
            'variable index'             => ['orderVariableIndex'],
            'variable expression index'  => ['orderVariableExpressionIndex'],
            'literal index'              => ['orderLiteralIndex'],
        ];
    }

    /**
     * PHP evaluates an index expression before the right-hand side, but reads
     * a plain variable index only when the element is written. The PHP twin
     * of every stub method is the same statement in plain PHP.
     *
     * @dataProvider orderProvider
     */
    public function testEvaluationOrder(string $method): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->{$method}(),
            static fn () => (new Issue2747Order())->{$method}(),
        );
    }
}
