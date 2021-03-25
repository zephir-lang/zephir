<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * CreateInstanceParamsOptimizer.
 *
 * Built-in function that creates new instances of objects from its class name passing parameters as an array
 */
class CreateInstanceParamsOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            throw new CompilerException('This function requires parameters', $expression);
        }

        if (2 != \count($expression['parameters'])) {
            throw new CompilerException('This function only requires two parameter', $expression);
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException('Returned values by functions can only be assigned to variant variables', $expression);
        }

        /*
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        $context->headersManager->add('kernel/object');

        $symbolVariable->setDynamicTypes('object');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }
        /*
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output('ZEPHIR_LAST_CALL_STATUS = zephir_create_instance_params('.$symbol.', '.$resolvedParams[0].', '.$resolvedParams[1].');');

        $call->addCallStatusOrJump($context);

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
