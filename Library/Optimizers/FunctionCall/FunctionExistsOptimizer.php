<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;
use Zephir\Utils;

/**
 * FunctionExistsOptimizer
 *
 * Optimizes calls to 'function_exists' using internal function
 */
class FunctionExistsOptimizer extends OptimizerAbstract
{
    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters']) || count($expression['parameters']) != 1) {
            return false;
        }

        if ($expression['parameters'][0]['parameter']['type'] == 'string') {
            $str = Utils::addSlashes($expression['parameters'][0]['parameter']['value']);
            unset($expression['parameters'][0]);
        }

        $context->headersManager->add('kernel/object');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        if (isset($str)) {
            /* TODO: Solve this macro stuff better, move to backend */
            $macro = $context->backend->isZE3() ? 'SL' : 'SS';
            return new CompiledExpression('bool', '(zephir_function_exists_ex(' . $macro . '("' . strtolower($str) . '") TSRMLS_CC) == SUCCESS)', $expression);
        }

        return new CompiledExpression('bool', '(zephir_function_exists(' . $resolvedParams[0] . ' TSRMLS_CC)  == SUCCESS)', $expression);
    }
}
