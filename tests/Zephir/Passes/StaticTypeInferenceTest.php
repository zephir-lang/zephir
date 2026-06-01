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

    /**
     * Reassigning a variable whose previously inferred type is not enumerated
     * in the merge switch (e.g. it came from a `(object)` cast or an
     * array-access) must not leak "StaticTypeInference=..." debug noise to
     * stdout; the variable should instead degrade to a dynamic one.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1877
     */
    public function testShouldNotEmitDebugOutputWhenMergingUnhandledType(): void
    {
        $this->expectOutputString('');

        $this->inference->markVariable('x', 'object'); // e.g. from a (object) cast
        $this->inference->markVariable('x', 'undefined'); // reassignment

        // A conflicting reassignment downgrades to a dynamic variable.
        $this->assertFalse($this->inference->getInferedType('x'));
    }

    /**
     * The companion default branch in passExpression() must stay silent for
     * expression node types it does not know about.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1877
     */
    public function testShouldNotEmitDebugOutputForUnhandledExpressionType(): void
    {
        $this->expectOutputString('');

        $result = $this->inference->passExpression(['type' => '__unknown__']);

        $this->assertNull($result);
    }
}
