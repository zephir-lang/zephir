<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompilerException;
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
        if (!isset($expression['parameters'])) {
            $context->codePrinter->output('zephir_exit_empty();');
        } else {
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
            $context->codePrinter->output('zephir_exit(' . $resolvedParams[0] .');');
        }
        $context->codePrinter->output('ZEPHIR_MM_RESTORE();');
        return new CompiledExpression('void ', '', $expression);
    }
}
