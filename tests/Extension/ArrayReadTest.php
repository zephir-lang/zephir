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
use Stub\ArrayRead;

use function fopen;

/**
 * `a[k]` in Zephir must read exactly as `$a[$k]` does in PHP 8: the same
 * warnings, deprecations and errors, in the same order, and the same value.
 * PHP is the oracle for every case, so per-version wording needs no
 * hardcoding.
 */
final class ArrayReadTest extends TestCase
{
    use AssertsPhpParity;

    private ArrayRead $test;

    protected function setUp(): void
    {
        $this->test = new ArrayRead();
    }

    public static function readProvider(): array
    {
        /* One resource for both sides, so its ID reads the same in each. */
        $resource = fopen('php://memory', 'r');

        return [
            'existing string key' => [static fn () => [['k' => 1], 'k']],
            'existing int key'    => [static fn () => [[1 => 2], 1]],
            'missing string key'  => [static fn () => [['k' => 1], 'zz']],
            'missing int key'     => [static fn () => [[1 => 2], 7]],
            'numeric string key'  => [static fn () => [[1 => 2], '1']],
            'null key'            => [static fn () => [['' => 3], null]],
            'float key'           => [static fn () => [[1 => 2], 1.5]],
            'bool key'            => [static fn () => [[1 => 2], true]],
            'resource key'        => [static fn () => [[1 => 2], $resource]],
            'array key'           => [static fn () => [[1 => 2], []]],
            'object key'          => [static fn () => [[1 => 2], new stdClass()]],
            'null container'      => [static fn () => [null, 'k']],
            'false container'     => [static fn () => [false, 'k']],
            'true container'      => [static fn () => [true, 'k']],
            'int container'       => [static fn () => [5, 'k']],
            'float container'     => [static fn () => [1.5, 'k']],
            'resource container'  => [static fn () => [$resource, 'k']],
            'string container'    => [static fn () => ['abc', 1]],
            'string missing'      => [static fn () => ['abc', 9]],
            'stdClass container'  => [static fn () => [new stdClass(), 'k']],
            'ArrayObject'         => [static fn () => [new ArrayObject(['k' => 1]), 'k']],
            'ArrayObject missing' => [static fn () => [new ArrayObject([]), 'k']],
            'SimpleXMLElement'    => [static fn () => [new SimpleXMLElement('<a b="c"/>'), 'b']],
            'reference element'   => [static function () {
                $x = 5;

                return [['k' => &$x], 'k'];
            }],
        ];
    }

    /**
     * @dataProvider readProvider
     */
    public function testReadMatchesPhp(callable $operands): void
    {
        $php = static function (callable $operands) {
            [$a, $k] = $operands();

            return $a[$k];
        };

        $this->assertMatchesPhp(fn () => $this->test->read(...$operands()), static fn () => $php($operands));
        $this->assertMatchesPhp(fn () => $this->test->readIntoLocal(...$operands()), static fn () => $php($operands));
    }

    public function testTypedArrayContainer(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->readFromArray(['k' => 1], 'zz'),
            static function () {
                $a = ['k' => 1];

                return $a['zz'];
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->readFromArray(['k' => 1], 'k'),
            static function () {
                $a = ['k' => 1];

                return $a['k'];
            },
        );
    }

    public static function literalProvider(): array
    {
        return [
            'array'          => [['zz' => 1, 7 => 2]],
            'missing'        => [['k' => 1]],
            'null container' => [null],
            'int container'  => [5],
            'string'         => ['abcdefgh'],
            'stdClass'       => [new stdClass()],
            'ArrayObject'    => [new ArrayObject(['zz' => 1])],
        ];
    }

    /**
     * Literal keys compile to the string and long fetchers.
     *
     * @dataProvider literalProvider
     */
    public function testLiteralKeys(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->readStringLiteral($a), static fn () => $a['zz']);
        $this->assertMatchesPhp(fn () => $this->test->readIntLiteral($a), static fn () => $a[7]);
    }

    /**
     * @dataProvider literalProvider
     */
    public function testNativeKeys(mixed $a): void
    {
        $this->assertMatchesPhp(fn () => $this->test->readLong($a, 7), static fn () => $a[7]);
        $this->assertMatchesPhp(fn () => $this->test->readString($a, 'zz'), static fn () => $a['zz']);
    }

    public function testNestedReads(): void
    {
        $a = ['k' => ['j' => 1], 's' => 5];

        $this->assertMatchesPhp(fn () => $this->test->readNested($a, 'k', 'j'), static fn () => $a['k']['j']);
        $this->assertMatchesPhp(fn () => $this->test->readNested($a, 'zz', 'j'), static fn () => $a['zz']['j']);
        $this->assertMatchesPhp(fn () => $this->test->readNested($a, 's', 'j'), static fn () => $a['s']['j']);
        $this->assertMatchesPhp(fn () => $this->test->readNestedLiteral($a), static fn () => $a['zz']['yy']);
    }
}
