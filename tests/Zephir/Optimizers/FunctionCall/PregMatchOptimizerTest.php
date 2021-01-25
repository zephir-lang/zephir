<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test\Optimizers\FunctionCall;

use PHPUnit\Framework\MockObject\MockObject;
use PHPUnit\Framework\TestCase;
use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Optimizers\FunctionCall\PregMatchOptimizer;
use Zephir\SymbolTable;
use Zephir\Variable;

class PregMatchOptimizerTest extends TestCase
{
    /** @var Call|MockObject */
    protected $callMock;

    /** @var CompilationContext|MockObject */
    protected $contextMock;

    /** @var MockObject|SymbolTable */
    protected $symTableMock;

    /** @var MockObject|Variable */
    protected $variableMock;

    protected function setUp(): void
    {
        $this->callMock = $this->createMock(Call::class);
        $this->contextMock = $this->createMock(CompilationContext::class);
        $this->symTableMock = $this->createMock(SymbolTable::class);
        $this->variableMock = $this->createMock(Variable::class);
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1697
     */
    public function testShouldThrowExceptionIfNoParametersGiven(): void
    {
        $this->expectException(\Zephir\Exception\CompilerException::class);
        $this->expectExceptionMessage('preg_match() expects at least 2 parameters, 0 given');

        $optimizer = new PregMatchOptimizer();

        $optimizer->optimize(
            [],
            $this->callMock,
            $this->contextMock
        );
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1697
     */
    public function testShouldThrowExceptionIfParametersLessThanRequired(): void
    {
        $this->expectException(\Zephir\Exception\CompilerException::class);
        $this->expectExceptionMessage('preg_match() expects at least 2 parameters, 0 given');

        $optimizer = new PregMatchOptimizer();

        $optimizer->optimize(
            ['parameters' => []],
            $this->callMock,
            $this->contextMock
        );
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1697
     */
    public function testShouldThrowExceptionIfParametersMoreThanRequired(): void
    {
        $this->expectException(\Zephir\Exception\CompilerException::class);
        $this->expectExceptionMessage('preg_match() expects at most 5 parameters, 7 given');

        $optimizer = new PregMatchOptimizer();

        $optimizer->optimize(
            ['parameters' => [0, 1, 2, 3, 4, 5, 6]],
            $this->callMock,
            $this->contextMock
        );
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1697
     */
    public function testShouldThrowExceptionIfMatchedIsNotVariable(): void
    {
        $this->expectException(\Zephir\Exception\CompilerException::class);
        $this->expectExceptionMessage('Only variables can be passed by reference');

        $optimizer = new PregMatchOptimizer();

        $expression = [
            'parameters' => [
                0 => [/* skip */],
                1 => [/* skip */],
                2 => ['parameter' => ['type' => 'int']],
            ],
        ];

        $optimizer->optimize(
            $expression,
            $this->callMock,
            $this->contextMock
        );
    }

    /**
     * @throws \ReflectionException
     * @issue https://github.com/phalcon/zephir/issues/1697
     */
    public function testShouldThrowExceptionIfUsedUndefinedMatchesVariable(): void
    {
        $this->expectException(\Zephir\Exception\CompilerException::class);
        $this->expectExceptionMessage("Cannot mutate variable 'matches' because it wasn't defined");

        $optimizer = new PregMatchOptimizer();

        $expression = [
            'parameters' => [
                0 => [/* skip */],
                1 => [/* skip */],
                2 => ['parameter' => ['type' => 'variable', 'value' => 'matches']],
            ],
        ];

        $context = new \ReflectionClass(CompilationContext::class);

        $this->symTableMock
            ->expects($this->once())
            ->method('getVariable')
            ->with('matches')
            ->willReturn(false);

        $symbolTable = $context->getProperty('symbolTable');
        $symbolTable->setValue($this->contextMock, $this->symTableMock);

        $optimizer->optimize(
            $expression,
            $this->callMock,
            $this->contextMock
        );
    }

    /**
     * @throws \ReflectionException
     * @issue https://github.com/phalcon/zephir/issues/1697
     */
    public function testShouldThrowExceptionIfMatchesHasUnexpectedType(): void
    {
        $this->expectException(\Zephir\Exception\CompilerException::class);
        $this->expectExceptionMessage("The 'matches' variable must be either a variable or an array, got Ooops");

        $optimizer = new PregMatchOptimizer();

        $expression = [
            'parameters' => [
                0 => [/* skip */],
                1 => [/* skip */],
                2 => ['parameter' => ['type' => 'variable', 'value' => 'matches']],
            ],
        ];

        $context = new \ReflectionClass(CompilationContext::class);

        $this->variableMock
            ->expects($this->exactly(2))
            ->method('getType')
            ->willReturn('Ooops');

        $this->symTableMock
            ->expects($this->once())
            ->method('getVariable')
            ->with('matches')
            ->willReturn($this->variableMock);

        $symbolTable = $context->getProperty('symbolTable');
        $symbolTable->setValue($this->contextMock, $this->symTableMock);

        $optimizer->optimize(
            $expression,
            $this->callMock,
            $this->contextMock
        );
    }
}
