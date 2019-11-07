<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\ClassDefinition;
use Zephir\ClassMethod;
use Zephir\Types;

class TypesTest extends TestCase
{
    private function baseClassDefinition(array $types): array
    {
        return [
            'type' => 'return-type',
            'list' => array_map(
                function ($type) {
                    return [
                        'type' => 'return-type-parameter',
                        'data-type' => $type,
                        'mandatory' => 0,
                        'file' => 'stubs.zep',
                        'line' => 1,
                        'char' => 42,
                    ];
                },
                $types,
            ),
        ];
    }

    private function objectClassDefinition(array $types): array
    {
        $ret = [];

        foreach ($types as $alias) {
            $ret[$alias] = 'Stubs\\'.$alias;
        }

        return $ret;
    }

    private function processVariableReturnTypes(array $types): array
    {
        return [
            'type' => 'return-type',
            'list' => array_map(
                function ($type) {
                    return [
                        'type' => 'return-type-parameter',
                        'cast' => [
                            'type' => 'variable',
                            'value' => $type,
                            'file' => 'stubs.zep',
                            'line' => 8,
                            'char' => 5,
                        ],
                        'collection' => 0,
                        'file' => 'stubs.zep',
                        'line' => 8,
                        'char' => 5,
                    ];
                },
                $types,
            ),
        ];
    }

    private function buildMethod(array $testData, string $definition): ClassMethod
    {
        return new ClassMethod(
            new ClassDefinition('Zephir', 'testMethod'),
            ['public'],
            'exampleMethodName',
            null,
            null,
            null,
            $this->$definition($testData),
        );
    }

    public function typesDataProvider(): array
    {
        return [
            [
                ['int'], 'int',
            ],
            [
                ['uint'], 'int',
            ],
            [
                ['char'], 'int',
            ],
            [
                ['uchar'], 'int',
            ],
            [
                ['long'], 'int',
            ],
            [
                ['ulong'], 'int',
            ],
            [
                ['double'], 'float',
            ],
            [
                ['float'], 'float',
            ],
            [
                ['number'], 'mixed',
            ],
            [
                ['null'], 'null',
            ],
            [
                ['bool'], 'bool',
            ],
            [
                ['string'], 'string',
            ],
            [
                ['istring'], 'string',
            ],
            [
                ['array'], 'array',
            ],
            [
                ['object'], 'object',
            ],
            [
                ['iterable'], 'iterable',
            ],
            [
                ['resource'], 'resource',
            ],
            [
                ['void'], 'void',
            ],
            [
                ['callable'], 'mixed',
            ],
            [
                ['var', 'null'], 'mixed|null',
            ],
            [
                ['string', 'null'], 'string|null',
            ],
            [
                ['char', 'ulong', 'int'], 'int',
            ],
        ];
    }

    /**
     * @test
     * @dataProvider typesDataProvider
     */
    public function shouldResolveCompatibleTypeForBaseTypes(array $returnTypes, string $expected)
    {
        $testMethod = $this->buildMethod($returnTypes, 'baseClassDefinition');
        $testTypes = new Types();

        $actual = $testTypes->getCompatibleReturnType($testMethod);

        $this->assertSame($actual, $expected);
    }

    public function objectsDataProvider(): array
    {
        return [
            [
                ['EventsManagerInterface'], 'EventsManagerInterface',
            ],
            [
                ['EventsManagerInterface', 'StdClass'], 'EventsManagerInterface|StdClass',
            ],
            [
                ['EventsManagerInterface', 'null'], 'EventsManagerInterface|null',
            ],
            [
                ['SomeNamespace\EventsManagerInterface', 'null'], 'SomeNamespace\EventsManagerInterface|null',
            ],
            [
                ['\SomeNamespace\EventsManagerInterface'], '\SomeNamespace\EventsManagerInterface',
            ],
        ];
    }

    /**
     * @test
     * @dataProvider objectsDataProvider
     */
    public function shouldResolveCompatibleTypeForObjects(array $returnTypes, string $expected)
    {
        // This processes into MethodDocBlock with AliasManager resolving
        $processedReturnTypes = $this->objectClassDefinition($returnTypes);

        $testMethod = $this->buildMethod($returnTypes, 'objectClassDefinition');

        $testMethod->setReturnTypes(
            $this->processVariableReturnTypes($returnTypes)
        );

        $testTypes = new Types();

        $actual = $testTypes->getCompatibleReturnType(
            $testMethod,
            $processedReturnTypes
        );

        $this->assertSame($actual, $expected);
    }
}
