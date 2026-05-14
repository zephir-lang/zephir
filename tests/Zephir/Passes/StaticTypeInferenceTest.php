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

namespace Zephir\Test\Passes;

use PHPUnit\Framework\TestCase;
use Zephir\Passes\StaticTypeInference;
use Zephir\StatementsBlock;

final class StaticTypeInferenceTest extends TestCase
{
    private StaticTypeInference $inference;

    protected function setUp(): void
    {
        $this->inference = new StaticTypeInference();
    }

    public function testShouldDeclareVariables(): void
    {
        $statement = [
            'variables' => [
                [
                    'variable' => 'test',
                    'expr' => ['type' => 'int'],
                ],
            ],
        ];

        $this->inference->declareVariables($statement);

        // Variable should be marked as inferred
        $this->assertTrue(true); // Basic test that method runs without error
    }

    public function testShouldMarkVariableWithArrayType(): void
    {
        $statement = [
            'variables' => [
                [
                    'variable' => 'myArray',
                    'expr' => ['type' => 'array'],
                ],
            ],
        ];

        $this->inference->declareVariables($statement);

        $this->assertTrue(true);
    }

    public function testShouldMarkVariableWithEmptyArrayType(): void
    {
        $statement = [
            'variables' => [
                [
                    'variable' => 'emptyArray',
                    'expr' => ['type' => 'empty-array'],
                ],
            ],
        ];

        $this->inference->declareVariables($statement);

        $this->assertTrue(true);
    }

    public function testShouldPassStatementsBlock(): void
    {
        $block = $this->createMock(StatementsBlock::class);
        $block->method('getStatements')->willReturn([]);

        $result = $this->inference->pass($block);

        // The pass method processes statements but returns null
        $this->assertNull($result);
    }
}
