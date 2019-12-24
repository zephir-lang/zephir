<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use function Zephir\add_slashes;
use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Compiler;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * MemnstrOptimizer.
 *
 * Like 'strpos' but it returns a boolean value
 */
class MemstrOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (2 != \count($expression['parameters'])) {
            return false;
        }

        if ('string' == $expression['parameters'][1]['parameter']['type']) {
            $str = add_slashes($expression['parameters'][1]['parameter']['value']);
            unset($expression['parameters'][1]);
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $context->headersManager->add('kernel/string');

        if (isset($str)) {
            return new CompiledExpression('bool', 'zephir_memnstr_str('.$resolvedParams[0].', SL("'.$str.'"), "'.Compiler::getShortUserPath($expression['file']).'", '.$expression['line'].')', $expression);
        }

        return new CompiledExpression('bool', 'zephir_memnstr('.$resolvedParams[0].', '.$resolvedParams[1].', "'.Compiler::getShortUserPath($expression['file']).'", '.$expression['line'].')', $expression);
    }
}
