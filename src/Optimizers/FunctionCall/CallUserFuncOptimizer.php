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
use Zephir\Optimizers\OptimizerAbstract;

use function count;

/**
 * CallUserFuncOptimizer.
 *
 * Optimizer for 'call_user_func'
 */
class CallUserFuncOptimizer extends OptimizerAbstract
{
    protected int    $parameterCount = 1;
    protected string $zephirMethod   = 'ZEPHIR_CALL_USER_FUNC';

    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if ($this->parameterCount !== count($expression['parameters'])) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariable($symbolVariable, $expression);
        if (!$symbolVariable) {
            $symbolVariable = $context->symbolTable->addTemp('variable', $context);
            $symbolVariable->initVariant($context);
        }

        /*
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        $resolvedParams = $call->getReadOnlyResolvedParams(
            $expression['parameters'],
            $context,
            $expression
        );

        $context->headersManager->add('kernel/fcall');

        /*
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);


        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output($this->getOutput($symbol, $resolvedParams));
        $call->addCallStatusOrJump($context);

        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
    }

    /**
     * @param string $symbol
     * @param array  $resolvedParams
     *
     * @return string
     */
    protected function getOutput(string $symbol, array $resolvedParams): string
    {
        return $this->zephirMethod
            . '(' . $symbol . ', ' . $resolvedParams[0] . ');';
    }
}
