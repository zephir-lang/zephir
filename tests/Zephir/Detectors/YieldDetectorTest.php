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

namespace Zephir\Test\Detectors;

use PHPUnit\Framework\TestCase;
use Zephir\Detectors\YieldDetector;

/**
 * @see https://github.com/zephir-lang/zephir/issues/1849
 */
final class YieldDetectorTest extends TestCase
{
    public function testFlatBodyWithoutYield(): void
    {
        $detector = new YieldDetector();

        $this->assertFalse($detector->detect([
            $this->literalStatement('return'),
        ]));
    }

    public function testFlatBodyWithYield(): void
    {
        $detector = new YieldDetector();

        $this->assertTrue($detector->detect([
            $this->yieldStatement(['type' => 'int', 'value' => '1']),
        ]));
    }

    public function testYieldNestedInLoop(): void
    {
        $detector = new YieldDetector();

        $statements = [
            [
                'type'       => 'while',
                'expr'       => ['type' => 'bool', 'value' => 'true'],
                'statements' => [
                    $this->yieldStatement(['type' => 'variable', 'value' => 'i']),
                ],
            ],
        ];

        $this->assertTrue($detector->detect($statements));
    }

    public function testYieldNestedInIfElseBranches(): void
    {
        $detector = new YieldDetector();

        $statements = [
            [
                'type'             => 'if',
                'expr'             => ['type' => 'variable', 'value' => 'cond'],
                'statements'       => [
                    $this->literalStatement('comment'),
                ],
                'else_statements'  => [
                    $this->yieldStatement(['type' => 'int', 'value' => '2']),
                ],
            ],
        ];

        $this->assertTrue($detector->detect($statements));
    }

    public function testYieldInsideSwitchClause(): void
    {
        $detector = new YieldDetector();

        $statements = [
            [
                'type'    => 'switch',
                'expr'    => ['type' => 'variable', 'value' => 'x'],
                'clauses' => [
                    [
                        'expr'       => ['type' => 'int', 'value' => '1'],
                        'statements' => [
                            $this->yieldStatement(['type' => 'int', 'value' => '42']),
                        ],
                    ],
                ],
            ],
        ];

        $this->assertTrue($detector->detect($statements));
    }

    public function testYieldInsideTryCatchBlock(): void
    {
        $detector = new YieldDetector();

        $statements = [
            [
                'type'       => 'try-catch',
                'statements' => [
                    $this->yieldStatement(['type' => 'int', 'value' => '3']),
                ],
                'catches'    => [],
            ],
        ];

        $this->assertTrue($detector->detect($statements));
    }

    public function testEmptyBody(): void
    {
        $detector = new YieldDetector();

        $this->assertFalse($detector->detect([]));
    }

    public function testBareYieldWithoutOperands(): void
    {
        $detector = new YieldDetector();

        $this->assertTrue($detector->detect([
            ['type' => 'yield', 'file' => 't.zep', 'line' => 1, 'char' => 1],
        ]));
    }

    public function testKeyValueYield(): void
    {
        $detector = new YieldDetector();

        $statements = [
            [
                'type'  => 'yield',
                'key'   => ['type' => 'string', 'value' => 'k'],
                'value' => ['type' => 'string', 'value' => 'v'],
                'file'  => 't.zep',
                'line'  => 1,
                'char'  => 1,
            ],
        ];

        $this->assertTrue($detector->detect($statements));
    }

    /**
     * @return array<string, mixed>
     */
    private function yieldStatement(array $expr): array
    {
        return [
            'type' => 'yield',
            'expr' => $expr,
            'file' => 't.zep',
            'line' => 1,
            'char' => 1,
        ];
    }

    /**
     * @return array<string, mixed>
     */
    private function literalStatement(string $type): array
    {
        return ['type' => $type, 'file' => 't.zep', 'line' => 1, 'char' => 1];
    }
}
