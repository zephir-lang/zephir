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
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;
use Zephir\Utils;

/**
 * MethodExistsOptimizer
 *
 * Optimizes calls to 'method_exists' using internal function
 */
class MethodExistsOptimizer extends OptimizerAbstract
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

        if ($expression['parameters'][1]['parameter']['type'] == 'string') {
            $str = Utils::addSlashes($expression['parameters'][1]['parameter']['value']);
            unset($expression['parameters'][1]);
        }

        $context->headersManager->add('kernel/object');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        /* TODO: Solve this macro stuff better, move to backend */
        $macro = $context->backend->isZE3() ? 'SL' : 'SS';
        if (isset($str)) {
            return new CompiledExpression('bool', '(zephir_method_exists_ex(' . $resolvedParams[0] . ', '. $macro . '("' . strtolower($str) . '") TSRMLS_CC) == SUCCESS)', $expression);
        }

        return new CompiledExpression('bool', '(zephir_method_exists(' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ' TSRMLS_CC)  == SUCCESS)', $expression);
    }
}
