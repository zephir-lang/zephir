<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * CallUserFuncArrayOptimizer
 *
 * Optimizer for 'call_user_func_array'
 */
class CallUserFuncArrayOptimizer extends OptimizerAbstract
{
    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (count($expression['parameters']) != 2) {
            return false;
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable) {
            if (!$symbolVariable->isVariable()) {
                throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
            }
        } else {
            $symbolVariable = $context->symbolTable->addTemp('variable', $context);
            $symbolVariable->initVariant($context);
        }

        /**
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $context->headersManager->add('kernel/fcall');

        /**
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output('ZEPHIR_CALL_USER_FUNC_ARRAY(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');');
        $call->addCallStatusOrJump($context);

        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
    }
}
