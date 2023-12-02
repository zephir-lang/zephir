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

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

use function count;

/**
 * CreateInstanceParamsOptimizer.
 *
 * Built-in function that creates new instances of objects from its class name passing parameters as an array
 */
class CreateInstanceParamsOptimizer extends OptimizerAbstract
{
    protected string $exceptionMessage = 'This function only requires two parameter';
    protected int    $parameterCount   = 2;

    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return CompiledExpression|mixed
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            throw new CompilerException('This function requires parameters', $expression);
        }

        if ($this->parameterCount !== count($expression['parameters'])) {
            throw new CompilerException(
                $this->exceptionMessage,
                $expression
            );
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariable($symbolVariable, $expression);

        /*
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        $context->headersManager->add('kernel/object');

        $symbolVariable->setDynamicTypes('object');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);

        /*
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output($this->getOutput($symbol, $resolvedParams));

        $this->getTempParameters($call, $context);
        $call->addCallStatusOrJump($context);

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * @param string $symbol
     * @param array  $resolvedParams
     *
     * @return string
     */
    protected function getOutput(string $symbol, array $resolvedParams): string
    {
        return 'ZEPHIR_LAST_CALL_STATUS = '
            . 'zephir_create_instance_params(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');'
        ;
    }
    /**
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return void
     */
    protected function getTempParameters(Call $call, CompilationContext $context): void
    {
        // Not needed here
    }
}
