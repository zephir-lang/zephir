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

namespace Zephir\Test\Class\Definition;

use PHPUnit\Framework\TestCase;
use ReflectionClass;
use ReflectionMethod;
use Zephir\Class\Definition\Definition;

/**
 * A class from a loaded extension is described through reflection. Its
 * parameter counts must match what PHP itself reports for the running
 * version, or calls PHP accepts are rejected at compile time.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2763
 */
final class BuildFromReflectionTest extends TestCase
{
    public static function internalMethodProvider(): array
    {
        return [
            'only a null default'           => ['SplFileInfo', 'getFileInfo'],
            'nullable int null default'     => ['ReflectionClass', 'getConstants'],
            'required then null default'    => ['SplObjectStorage', 'attach'],
            'null default after scalars'    => ['Exception', '__construct'],
            'string then null default'      => ['DateTime', '__construct'],
            'two required then null'        => ['DateTime', 'createFromFormat'],
            'only a variadic'               => ['ReflectionFunction', 'invoke'],
            'required then variadic'        => ['ReflectionMethod', 'invoke'],
        ];
    }

    /**
     * @dataProvider internalMethodProvider
     */
    public function testRequiredParameterCountMatchesPhp(string $class, string $method): void
    {
        $definition = Definition::buildFromReflection(new ReflectionClass($class));

        $this->assertSame(
            (new ReflectionMethod($class, $method))->getNumberOfRequiredParameters(),
            $definition->getMethod($method)->getNumberOfRequiredParameters()
        );
    }

    /**
     * @dataProvider internalMethodProvider
     */
    public function testVariadicFlagMatchesPhp(string $class, string $method): void
    {
        $definition = Definition::buildFromReflection(new ReflectionClass($class));

        $this->assertSame(
            (new ReflectionMethod($class, $method))->isVariadic(),
            $definition->getMethod($method)->isVariadic()
        );
    }
}
