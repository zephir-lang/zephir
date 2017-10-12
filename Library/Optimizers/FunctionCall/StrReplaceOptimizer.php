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
 * StrReplaceOptimizer
 *
 * Optimizes calls to 'str_replace' using internal function
 */
class StrReplaceOptimizer extends OptimizerAbstract
{
    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return bool|CompiledExpression|mixed
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (count($expression['parameters']) != 3) {
            if (count($expression['parameters']) == 4) {
                return false;
            }
            throw new CompilerException("'str_replace' only accepts three parameter", $expression);
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
        }

        $context->headersManager->add('kernel/string');

        $symbolVariable->setDynamicTypes('string');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if ($call->mustInitSymbolVariable()) {
            if ($symbolVariable->getName() == 'return_value') {
                $symbolVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
            } else {
                $symbolVariable->initVariant($context);
            }
        }

        $symbol = $context->backend->getVariableCodePointer($symbolVariable);

        $context->codePrinter->output('zephir_fast_str_replace(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $resolvedParams[2] . ' TSRMLS_CC);');

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
