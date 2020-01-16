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

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * MtRandOptimizer.
 *
 * Optimizes calls to 'mt_rand' using internal function
 */
class MtRandOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @throws CompilerException
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

        $context->headersManager->add('kernel/math');
        $context->headersManager->add('kernel/operators');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        return new CompiledExpression('long', 'zephir_mt_rand(zephir_get_intval('.$resolvedParams[0].'), zephir_get_intval('.$resolvedParams[1].'))', $expression);
    }
}
