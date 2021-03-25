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

namespace Zephir\Test\FunctionLike\ReturnType;

use PHPUnit\Framework\TestCase;
use Zephir\FunctionLike\ReturnType\CastHint;
use Zephir\FunctionLike\ReturnType\Factory;
use Zephir\FunctionLike\ReturnType\RealType;
use Zephir\FunctionLike\ReturnType\TypeInterface;
use Zephir\Types;

final class FactoryTest extends TestCase
{
    /**
     * @dataProvider realTypeDataProvider
     *
     * @param array $ir
     */
    public function testShouldCreateRealType(array $ir): void
    {
        $factory = new Factory();
        $this->assertInstanceOf(RealType::class, $factory->create($ir));
    }

    public function realTypeDataProvider(): array
    {
        return [
            [
                [
                    'data-type' => Types::T_STRING,
                    'mandatory' => 0,
                    'type' => TypeInterface::TYPE_PARAMETER,
                ],
            ],
            [
                [
                    'data-type' => Types::T_DOUBLE,
                    'mandatory' => 1,
                    'type' => TypeInterface::TYPE_ANNOTATION,
                ],
            ],
            [
                [
                    Types::T_INT,
                ],
            ],
            [
                [
                    Types::T_UNDEFINED,
                ],
            ],
        ];
    }

    /**
     * @dataProvider castHintDataProvider
     *
     * @param array $ir
     */
    public function testShouldCreateCastHint(array $ir): void
    {
        $factory = new Factory();
        $this->assertInstanceOf(CastHint::class, $factory->create($ir));
    }

    public function castHintDataProvider(): array
    {
        return [
            [
                [
                    'cast' => [
                        'type' => Types::T_VARIABLE,
                        'mandatory' => 0,
                    ],
                    'type' => TypeInterface::TYPE_PARAMETER,
                ],
            ],
            [
                [
                    'cast' => [
                        'type' => Types::T_VARIABLE,
                        'value' => 'Class1',
                    ],
                    'type' => TypeInterface::TYPE_ANNOTATION,
                    'collection' => 0,
                ],
            ],
            [
                [
                    'cast' => [
                        'value' => 'Class1',
                    ],
                    'collection' => 1,
                ],
            ],
            [
                [
                    'cast' => [
                        'value' => null,
                    ],
                    'type' => Types::T_INT,
                ],
            ],
            [
                [
                    'cast' => [],
                ],
            ],
        ];
    }
}
