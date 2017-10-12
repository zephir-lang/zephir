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
 * ObStartOptimizer
 *
 * Optimizes calls to 'ob_start' using internal function
 */
class ObStart extends OptimizerAbstract
{
    /**
     * @todo This optimizer doesn't work at while and etc statements
     *
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

        if (count($expression['parameters']) > 0) {
            return false;
        }

        $context->headersManager->add('kernel/array');
        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        return new CompiledExpression('null', 'zephir_ob_start(TSRMLS_C)', $expression);
    }
}
