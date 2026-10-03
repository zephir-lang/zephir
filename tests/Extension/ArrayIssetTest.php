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
use PHPUnit\Framework\TestCase;
use SimpleXMLElement;
use stdClass;
use Stub\ArrayIsset;

use function fopen;

/**
 * `isset a[k]` and `empty(a[k])` in Zephir must behave as `isset($a[$k])` and
 * `empty($a[$k])` do in PHP 8: the same offset conversion diagnostics, the same
 * TypeError for an illegal offset, the same answer. PHP is the oracle.
 */
final class ArrayIssetTest extends TestCase
{
    use AssertsPhpParity;

    private ArrayIsset $test;

    protected function setUp(): void
    {
        $this->test = new ArrayIsset();
    }

    public static function issetProvider(): array
    {
        /* One resource for both sides, so its ID reads the same in each. */
        $resource = fopen('php://memory', 'r');
        $array    = [1 => 'x', '' => 'e', 'k' => 0];

        return [
            'int key'            => [static fn () => [$array, 1]],
            'missing key'        => [static fn () => [$array, 9]],
            'string key'         => [static fn () => [$array, 'k']],
            'numeric string key' => [static fn () => [$array, '1']],
            'null key'           => [static fn () => [$array, null]],
            'float key'          => [static fn () => [$array, 1.5]],
            'bool key'           => [static fn () => [$array, true]],
            'resource key'       => [static fn () => [$array, $resource]],
            'array key'          => [static fn () => [$array, []]],
            'object key'         => [static fn () => [$array, new stdClass()]],
            'string container'   => [static fn () => ['abc', 1]],
            'string float key'   => [static fn () => ['abc', 1.5]],
            'string word key'    => [static fn () => ['abc', 'x']],
            'string array key'   => [static fn () => ['abc', []]],
            'null container'     => [static fn () => [null, 'k']],
            'int container'      => [static fn () => [5, 'k']],
            'stdClass'           => [static fn () => [new stdClass(), 'k']],
            'SimpleXMLElement'   => [static fn () => [new SimpleXMLElement('<a b="c"/>'), 'b']],
            'ArrayObject'        => [static fn () => [new ArrayObject(['k' => 1]), 'k']],
            'ArrayObject array'  => [static fn () => [new ArrayObject([]), []]],
        ];
    }

    /**
     * @dataProvider issetProvider
     */
    public function testIssetAndEmpty(callable $operands): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->issetVar(...$operands()),
            static function () use ($operands) {
                [$a, $k] = $operands();

                return isset($a[$k]);
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->emptyVar(...$operands()),
            static function () use ($operands) {
                [$a, $k] = $operands();

                return empty($a[$k]);
            },
        );
    }

    /**
     * `fetch` is isset that also reads; with no null element the answers
     * coincide, so isset is the oracle for its diagnostics and its result.
     *
     * @dataProvider issetProvider
     */
    public function testFetch(callable $operands): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->fetchVar(...$operands()),
            static function () use ($operands) {
                [$a, $k] = $operands();

                return isset($a[$k]);
            },
        );
    }

    public function testLiteralAndNestedForms(): void
    {
        $a = ['zz' => 1, 1 => ['j' => 2]];

        $this->assertMatchesPhp(fn () => $this->test->issetStringLiteral($a), static fn () => isset($a['zz']));
        $this->assertMatchesPhp(fn () => $this->test->issetIntLiteral($a), static fn () => isset($a[1]));
        $this->assertMatchesPhp(fn () => $this->test->issetNested($a, 1, 'j'), static fn () => isset($a[1]['j']));
        $this->assertMatchesPhp(fn () => $this->test->issetNested($a, 1, []), static fn () => isset($a[1][[]]));
        $this->assertMatchesPhp(fn () => $this->test->issetNested($a, 9, 'j'), static fn () => isset($a[9]['j']));

        $o = new stdClass();
        $this->assertMatchesPhp(fn () => $this->test->issetStringLiteral($o), static fn () => isset($o['zz']));
        $this->assertMatchesPhp(fn () => $this->test->issetIntLiteral($o), static fn () => isset($o[1]));
    }
}
