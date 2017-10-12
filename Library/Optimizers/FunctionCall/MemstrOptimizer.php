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
use Zephir\Compiler;

/**
 * MemnstrOptimizer
 *
 * Like 'strpos' but it returns a boolean value
 */
class MemstrOptimizer extends OptimizerAbstract
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

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $context->headersManager->add('kernel/string');

        if (isset($str)) {
            return new CompiledExpression('bool', 'zephir_memnstr_str(' . $resolvedParams[0] . ', SL("' . $str . '"), "' . Compiler::getShortUserPath($expression['file']) . '", ' . $expression['line'] . ')', $expression);
        }

        return new CompiledExpression('bool', 'zephir_memnstr(' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', "' . Compiler::getShortUserPath($expression['file']) . '", ' . $expression['line'] . ')', $expression);
    }
}
