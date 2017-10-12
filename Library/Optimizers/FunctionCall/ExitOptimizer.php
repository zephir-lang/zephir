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
 * ExitOptimizer
 *
 * Optimizes calls to 'exit' using internal function
 */
class ExitOptimizer extends OptimizerAbstract
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
        if (isset($expression['parameters']) && count($expression['parameters']) > 1) {
            return false;
        }

        $context->headersManager->add('kernel/exit');
        if (isset($expression['parameters'])) {
            //TODO: protect resolvedParams[0] from restore
        }
        if (!isset($expression['parameters'])) {
            $context->codePrinter->output('ZEPHIR_MM_RESTORE();');
            $context->codePrinter->output('zephir_exit_empty();');
        } else {
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
            $context->codePrinter->output('ZEPHIR_MM_RESTORE();');
            $context->codePrinter->output('zephir_exit(' . $resolvedParams[0] .');');
        }
        return new CompiledExpression('void ', '', $expression);
    }
}
