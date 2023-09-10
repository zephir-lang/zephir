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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Class\ClassDefinition;
use Zephir\Class\ClassMethod;
use Zephir\Types;

final class TypesTest extends TestCase
{
    /**
     * Helper to build proper structure with method return types.
     *
     * @param array $returnTypesList - collactions with all return types
     *
     * @return array
     */
    private function buildReturnTypes(array $returnTypesList): array
    {
        return [
            'type' => 'return-type',
            'list' => $returnTypesList,
            'void' => 0,
            'file' => 'stubs.zep',
            'line' => 1,
            'char' => 42,
        ];
    }

    /**
     * Builds base object definition for return type.
     *
     * @param array $types     - list of method return types
     * @param int   $mandatory - is mandatory flag
     *
     * @return array
     */
    private function baseReturnTypeDefinition(array $types, int $mandatory = 0): array
    {
        return array_map(
            function ($type) use ($mandatory) {
                return [
                    'type' => 'return-type-parameter',
                    'data-type' => $type,
                    'mandatory' => $mandatory,
                    'file' => 'stubs.zep',
                    'line' => 1,
                    'char' => 42,
                ];
            },
            $types
        );
    }

    /**
     * Builds variable/collection object definition for return type.
     *
     * @param array $types      - list of method return types
     * @param int   $collection - is collection flag
     *
     * @return array
     */
    private function variableReturnTypeDefinition(array $types, int $collection = 0): array
    {
        return array_map(
            function ($type) use ($collection) {
                return [
                    'type' => 'return-type-parameter',
                    'cast' => [
                        'type' => 'variable',
                        'value' => $type,
                        'file' => 'stubs.zep',
                        'line' => 8,
                        'char' => 5,
                    ],
                    'collection' => $collection,
                    'file' => 'stubs.zep',
                    'line' => 8,
                    'char' => 5,
                ];
            },
            $types
        );
    }

    /**
     * Helper to build test Method with injected test data.
     *
     * @param array $testData   - dataProvider data set
     * @param int   $definition - (optional) one of mandatory/collection flag
     */
    private function buildMethod(array $testData, int $definition = 0): ClassMethod
    {
        return new ClassMethod(
            new ClassDefinition('Zephir', 'testMethod'),
            ['public'],
            'exampleMethodName',
            null,
            null,
            null,
            $this->buildReturnTypes($testData, $definition)
        );
    }

    public function typesProvider(): array
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
            [
                ['mixed', 'string'], 'mixed|string',
            ],
        ];
    }

    /**
     * @dataProvider typesProvider
     *
     * @param array  $returnTypes
     * @param string $expected
     */
    public function testShouldResolveCompatibleTypeForBaseTypes(array $returnTypes, string $expected): void
    {
        $testMethod = $this->buildMethod(
            $this->baseReturnTypeDefinition($returnTypes)
        );

        $testTypes = new Types();
        $actual = $testTypes->getReturnTypeAnnotation($testMethod);

        $this->assertSame($expected, $actual);
    }

    public function objectsProvider(): array
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
     * @dataProvider objectsProvider
     *
     * @param array  $returnTypes
     * @param string $expected
     */
    public function testShouldResolveCompatibleTypeForObjects(array $returnTypes, string $expected): void
    {
        $withAliases = [];
        foreach ($returnTypes as $alias) {
            $withAliases[$alias] = 'Stubs\\'.$alias;
        }

        $testMethod = $this->buildMethod(
            $this->baseReturnTypeDefinition($returnTypes)
        );

        $testTypes = new Types();

        $actual = $testTypes->getReturnTypeAnnotation(
            $testMethod,
            $withAliases
        );

        $this->assertSame($expected, $actual);
    }

    public function collectionsProvider(): array
    {
        return [
            //  [ Zephir return types...], 'expected php docblock param'
            '<\StdClass[]> | bool' => [
                ['<\StdClass[]>', 'bool'], 'array|bool|\StdClass[]',
            ],
            '<IndexInterface[]>' => [
                ['<IndexInterface[]>'], 'array|IndexInterface[]',
            ],
            '<RelationInterface[]> | array' => [
                ['<RelationInterface[]>', 'array'], 'array|RelationInterface[]',
            ],
            '<RelationInterface[]> | <Item[]> | array' => [
                ['<RelationInterface[]>', '<Item[]>', 'array'], 'array|RelationInterface[]|Item[]',
            ],
        ];
    }

    /**
     * @dataProvider collectionsProvider
     *
     * @param array  $returnTypes
     * @param string $expected
     */
    public function testShouldResolveCompatibleTypeForCollections(array $returnTypes, string $expected): void
    {
        $typesList = [];
        $collections = [];
        foreach ($returnTypes as $type) {
            if (false !== strpos($type, '[]')) {
                $typesList[] = $this->variableReturnTypeDefinition([$type], 1)[0];
                $collectionType = trim($type, '<>');
                $collections[$collectionType] = $collectionType;
            } else {
                $typesList[] = $this->baseReturnTypeDefinition([$type])[0];
            }
        }

        $testMethod = $this->buildMethod($typesList);
        $testTypes = new Types();
        $actual = $testTypes->getReturnTypeAnnotation(
            $testMethod,
            array_merge($testMethod->getReturnTypes(), $collections)
        );

        $this->assertSame($expected, $actual);
    }
}
