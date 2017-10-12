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
 * FileGetContentsOptimizer
 *
 * Optimizes calls to 'file_get_contents' using internal function
 */
class FileGetContentsOptimizer extends OptimizerAbstract
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

        if (count($expression['parameters']) != 1) {
            return false;
        }

        $context->headersManager->add('kernel/file');

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable) {
            if ($symbolVariable->isNotVariableAndString()) {
                throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
            }
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }
        if ($symbolVariable) {
            $symbol = $context->backend->getVariableCode($symbolVariable);
            $context->codePrinter->output('zephir_file_get_contents(' . $symbol . ', ' . $resolvedParams[0] . ' TSRMLS_CC);');
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        } else {
            $context->codePrinter->output('zephir_file_get_contents(NULL, ' . $resolvedParams[0] . ' TSRMLS_CC);');
        }

        return new CompiledExpression('null', 'null', $expression);
    }
}
