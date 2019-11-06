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
    public function buildMethod(array $testData): ClassMethod
    {
        return new ClassMethod(
            new ClassDefinition('Zephir', 'testMethod'),
            ['public'],
            'exampleMethodName',
            null,
            null,
            null,
            [
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
                    $testData
                ),
            ]
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
        ];
    }

    /**
     * @test
     * @dataProvider typesDataProvider
     */
    public function integerTypeCompatible(array $returnTypes, string $expected)
    {
        $testMethod = $this->buildMethod($returnTypes);
        $testTypes = new Types();

        $actual = $testTypes->getCompatibleReturnType($testMethod);

        $this->assertSame($actual, $expected);
    }
}
