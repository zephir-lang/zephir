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
use ArrayTypedRefHolder;
use Issue2747Holder;
use PHPUnit\Framework\TestCase;
use stdClass;
use Stub\ArrayWrite;
use Stub\Issue2747;

use function fopen;

use const PHP_INT_MAX;

/**
 * `a[k] = v` in Zephir must write exactly as `$a[$k] = $v` does in PHP 8: the
 * same diagnostics, the same errors and the same resulting container. PHP is
 * the oracle for every case.
 */
final class ArrayWriteTest extends TestCase
{
    use AssertsPhpParity;

    private ArrayWrite $test;

    protected function setUp(): void
    {
        $this->test = new ArrayWrite();
    }

    public static function containerProvider(): array
    {
        /* One resource for both sides, so its ID reads the same in each. */
        $resource = fopen('php://memory', 'r');

        return [
            'array'              => [static fn () => ['x' => 1]],
            'empty array'        => [static fn () => []],
            'null'               => [static fn () => null],
            'false'              => [static fn () => false],
            'true'               => [static fn () => true],
            'int'                => [static fn () => 5],
            'float'              => [static fn () => 1.5],
            'empty string'       => [static fn () => ''],
            'string'             => [static fn () => 'abc'],
            'resource'           => [static fn () => $resource],
            'stdClass'           => [static fn () => new stdClass()],
            'ArrayObject'        => [static fn () => new ArrayObject([])],
            'full array'         => [static fn () => [PHP_INT_MAX => 1]],
            'reference element'  => [static function () {
                $x = [];

                return ['r' => &$x];
            }],
        ];
    }

    /**
     * @dataProvider containerProvider
     */
    public function testWriteForms(callable $container): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->write($container(), 1, 'v'),
            static function () use ($container) {
                $a    = $container();
                $a[1] = 'v';

                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->writeStringLiteral($container(), 'v'),
            static function () use ($container) {
                $a      = $container();
                $a['k'] = 'v';

                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->writeIntLiteral($container(), 'v'),
            static function () use ($container) {
                $a    = $container();
                $a[1] = 'v';

                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->writeLong($container(), 2, 'v'),
            static function () use ($container) {
                $a    = $container();
                $a[2] = 'v';

                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->writeNative($container(), 'k'),
            static function () use ($container) {
                $a      = $container();
                $a['k'] = 5;

                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->writeNested($container(), 'i', 'j', 'v'),
            static function () use ($container) {
                $a           = $container();
                $a['i']['j'] = 'v';

                return $a;
            },
        );
    }

    /**
     * @dataProvider containerProvider
     */
    public function testAppendForms(callable $container): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->append($container(), 'v'),
            static function () use ($container) {
                $a   = $container();
                $a[] = 'v';

                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->appendNative($container()),
            static function () use ($container) {
                $a   = $container();
                $a[] = 5;

                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->appendNested($container(), 'k', 'v'),
            static function () use ($container) {
                $a        = $container();
                $a['k'][] = 'v';

                return $a;
            },
        );
    }

    /**
     * @dataProvider containerProvider
     */
    public function testPropertyForms(callable $container): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->writeThis($container(), 'k', 'v'),
            static function () use ($container) {
                $p      = $container();
                $p['k'] = 'v';

                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->writeThisNested($container(), 'i', 'j', 'v'),
            static function () use ($container) {
                $p           = $container();
                $p['i']['j'] = 'v';

                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->appendThis($container(), 'v'),
            static function () use ($container) {
                $p   = $container();
                $p[] = 'v';

                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->writeStatic($container(), 'k', 'v'),
            static function () use ($container) {
                $p      = $container();
                $p['k'] = 'v';

                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->appendStatic($container(), 'v'),
            static function () use ($container) {
                $p   = $container();
                $p[] = 'v';

                return $p;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->writeObject(new Issue2747Holder($container()), 'k', 'v'),
            static function () use ($container) {
                $o         = new Issue2747Holder($container());
                $o->p['k'] = 'v';

                return $o;
            },
        );
    }

    public static function keyProvider(): array
    {
        /* One resource for both sides, so its ID reads the same in each. */
        $resource = fopen('php://memory', 'r');

        return [
            'int'            => [static fn () => 1],
            'numeric string' => [static fn () => '1'],
            'string'         => [static fn () => 'k'],
            'null'           => [static fn () => null],
            'float'          => [static fn () => 1.5],
            'true'           => [static fn () => true],
            'resource'       => [static fn () => $resource],
            'array'          => [static fn () => []],
            'object'         => [static fn () => new stdClass()],
        ];
    }

    /**
     * @dataProvider keyProvider
     */
    public function testKeyConversion(callable $key): void
    {
        $this->assertMatchesPhp(
            fn () => $this->test->write([], $key(), 'v'),
            static function () use ($key) {
                $a          = [];
                $a[$key()]  = 'v';

                return $a;
            },
        );
        $this->assertMatchesPhp(
            fn () => $this->test->writeThis([], $key(), 'v'),
            static function () use ($key) {
                $p          = [];
                $p[$key()]  = 'v';

                return $p;
            },
        );
    }

    public static function typedReferenceProvider(): array
    {
        return [
            '?int'      => ['i'],
            '?array'    => ['a'],
            'int|false' => ['f'],
            'mixed'     => ['m'],
        ];
    }

    /**
     * An element that is a reference to a typed property may become an array
     * only if the type allows one. PHP checks every write context except the
     * last offset of a compound assignment, which it lets through.
     *
     * @dataProvider typedReferenceProvider
     */
    public function testTypedReferenceAutoInitialization(string $property): void
    {
        $container = static function () use ($property) {
            $holder = new ArrayTypedRefHolder();

            return [['k' => &$holder->{$property}], $holder];
        };

        $this->assertMatchesPhp(
            function () use ($container) {
                [$a, $holder] = $container();

                return [$this->test->writeNested($a, 'k', 'j', 1), $holder];
            },
            static function () use ($container) {
                [$a, $holder] = $container();
                $a['k']['j'] = 1;

                return [$a, $holder];
            },
        );
        $this->assertMatchesPhp(
            function () use ($container) {
                [$a, $holder] = $container();

                return [$this->test->appendNested($a, 'k', 1), $holder];
            },
            static function () use ($container) {
                [$a, $holder] = $container();
                $a['k'][] = 1;

                return [$a, $holder];
            },
        );
        $this->assertMatchesPhp(
            function () use ($container) {
                [$a, $holder] = $container();

                return [(new Issue2747())->addNested($a, 'k', 'j', 1), $holder];
            },
            static function () use ($container) {
                [$a, $holder] = $container();
                $a['k']['j'] += 1;

                return [$a, $holder];
            },
        );
        $this->assertMatchesPhp(
            function () use ($container) {
                [$a, $holder] = $container();

                return [$this->test->concatDeep($a, 'k', 'j', 'l', 'v'), $holder];
            },
            static function () use ($container) {
                [$a, $holder] = $container();
                $a['k']['j']['l'] .= 'v';

                return [$a, $holder];
            },
        );
    }
}
