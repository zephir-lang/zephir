<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\MethodsWithDefaultValues;
use ReflectionClass;

final class MethodPropertiesDefaultValuesReflectionTest extends TestCase
{
    /**
     * @dataProvider methodsProvider
     */
    public function testDefaultValues(string $method, mixed $expected): void
    {
        $reflection = new ReflectionClass(MethodsWithDefaultValues::class);

        $parameterReflection = $reflection->getMethod($method)->getParameters()[0];
        $this->assertTrue(
            $parameterReflection->isDefaultValueAvailable(),
            'Default value is not set'
        );

        $this->assertSame($expected, $parameterReflection->getDefaultValue());
    }

    public function methodsProvider(): array
    {
        return [
            ['testBool', true],
            ['testNullableBool', null],
            ['testString', "va'lue"],
            ['testNullableString', null],
            ['testInt', 1337],
            ['testNullableInt', null],
            ['testDouble', 13.37],
            ['testNullableDouble', null],
        ];
    }
}
