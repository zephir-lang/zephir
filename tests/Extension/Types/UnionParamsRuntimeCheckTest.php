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

namespace Extension\Types;

use ArrayObject;
use Extension\Types\UnionParams\ArrayObjectChild;
use Extension\Types\UnionParams\PhpUnionParams;
use Extension\Types\UnionParams\TextValue;
use Extension\Types\UnionParams\WeakModeCaller;
use PHPUnit\Framework\TestCase;
use stdClass;
use Stub\Types\MayBe;
use Stub\Types\UnionParams;
use Throwable;

/**
 * A union-typed parameter accepts, coerces and rejects arguments on entry
 * exactly as the same parameter written in PHP, in both typing modes.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2761
 */
final class UnionParamsRuntimeCheckTest extends TestCase
{
    /**
     * @dataProvider argumentProvider
     */
    public function testStrictModeMatchesPhp(string $method, array $arguments): void
    {
        $this->assertSame(
            $this->outcome(static fn () => (new PhpUnionParams())->{$method}(...$arguments)),
            $this->outcome(static fn () => (new UnionParams())->{$method}(...$arguments)),
        );
    }

    /**
     * @dataProvider argumentProvider
     */
    public function testWeakModeMatchesPhp(string $method, array $arguments): void
    {
        $this->assertSame(
            $this->outcome(static fn () => WeakModeCaller::call([new PhpUnionParams(), $method], $arguments)),
            $this->outcome(static fn () => WeakModeCaller::call([new UnionParams(), $method], $arguments)),
        );
    }

    public function argumentProvider(): array
    {
        $maybe = new MayBe();

        return [
            'int to int|float'              => ['number', [5]],
            'float to int|float'            => ['number', [2.5]],
            'numeric string to int|float'   => ['number', ['5']],
            'float string to int|float'     => ['number', ['5.5']],
            'text to int|float'             => ['number', ['abc']],
            'leading numeric to int|float'  => ['number', ['5 apples']],
            'bool to int|float'             => ['number', [true]],
            'array to int|float'            => ['number', [[]]],
            'object to int|float'           => ['number', [new stdClass()]],
            'int to string|bool'            => ['scalar', [5]],
            'float to string|bool'          => ['scalar', [1.5]],
            'stringable to string|bool'     => ['scalar', [new TextValue()]],
            'int to float|string'           => ['floatOrString', [5]],
            'bool to int|false'             => ['intOrFalse', [true]],
            'false to int|false'            => ['intOrFalse', [false]],
            'string to int|false'           => ['intOrFalse', ['7']],
            'class to class|array'          => ['objectOrArray', [$maybe]],
            'array to class|array'          => ['objectOrArray', [[1]]],
            'other object to class|array'   => ['objectOrArray', [new stdClass()]],
            'string to class|array'         => ['objectOrArray', ['x']],
            'first class of two'            => ['classes', [$maybe]],
            'second class of two'           => ['classes', [new ArrayObject()]],
            'subclass of second class'      => ['classes', [new ArrayObjectChild()]],
            'other object to two classes'   => ['classes', [new stdClass()]],
            'second argument'               => ['second', ['x', 'abc']],
            'second argument coerced'       => ['second', ['x', '7']],
            'optional omitted'              => ['optional', []],
            'optional given wrong type'     => ['optional', [[]]],
            'null default given null'       => ['nullDefault', [null]],
            'null default given wrong type' => ['nullDefault', [1.5]],
        ];
    }

    public function testStaticMethodIsChecked(): void
    {
        $this->assertSame(
            $this->outcome(static fn () => PhpUnionParams::staticNumber('abc')),
            $this->outcome(static fn () => UnionParams::staticNumber('abc')),
        );
    }

    public function testStrictNullIsRejected(): void
    {
        $this->assertSame(
            'TypeError: CLASS::number(): Argument #1 ($value) must be of type int|float, null given',
            $this->outcome(static fn () => (new UnionParams())->number(null)),
        );
    }

    /**
     * Like any internal function, a weak-mode null reaching a scalar
     * parameter is coerced, with a deprecation from PHP 8.1.
     */
    public function testWeakNullIsCoercedLikeAnInternalFunction(): void
    {
        $deprecations = [];
        set_error_handler(static function (int $errno, string $message) use (&$deprecations): bool {
            $deprecations[] = $message;

            return true;
        }, E_DEPRECATED);

        try {
            $result = WeakModeCaller::call([new UnionParams(), 'number'], [null]);
        } finally {
            restore_error_handler();
        }

        $this->assertSame(0, $result);
        $this->assertSame(
            PHP_VERSION_ID >= 80100
                ? ['Stub\Types\UnionParams::number(): Passing null to parameter #1 ($value) of type int|float is deprecated']
                : [],
            $deprecations,
        );
    }

    /**
     * The returned value, or the thrown error with the class name and the
     * userland-only `, called in` suffix normalised away.
     */
    private function outcome(callable $call): string
    {
        try {
            return var_export($call(), true);
        } catch (Throwable $e) {
            $message = preg_replace('/, called in .* on line \d+$/', '', $e->getMessage());

            return get_class($e) . ': ' . str_replace(
                [PhpUnionParams::class, UnionParams::class],
                'CLASS',
                $message
            );
        }
    }
}
