<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * FuncNumArgsOptimizer.
 *
 * Optimizes calls to 'func_num_args' using internal function
 */
class FuncNumArgsOptimizer extends OptimizerAbstract
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
        return new CompiledExpression('int', 'ZEND_NUM_ARGS()', $expression);
    }
}
