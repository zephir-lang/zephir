<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test\Detectors;

use PHPUnit\Framework\TestCase;
use Zephir\Detectors\ReadDetector;

class ReadDetectorTest extends TestCase
{
    /**
     * @test
     * @dataProvider variableProvider
     */
    public function shouldDetectsIfVariableIsUsedInGivenExpressionContext(bool $expected, string $variable, array $expression)
    {
        $detector = new ReadDetector();

        $this->assertSame($expected, $detector->detect($variable, $expression));
    }

    public function variableProvider(): array
    {
        $variable = [
            'type' => 'variable',
            'value' => 'foo',
        ];

        $fcall = [
            'type' => 'fcall',
            'parameters' => [
                [
                    'parameter' => [
                        'type' => 'variable',
                        'value' => 'foo',
                        'file' => '',
                        'line' => 1,
                        'char' => 1,
                    ],
                ],
            ],
        ];

        return [
            'no type' => [false, 'foo', []],
            'type variable' => [true, 'foo', $variable],
            'type magic variable' => [true, 'foo$$', $variable],
            'type function' => [true, 'foo', $fcall],
        ];
    }
}
