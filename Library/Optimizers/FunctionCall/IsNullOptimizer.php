<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Optimizers\IsTypeOptimizerAbstract;

/**
 * IsNullOptimizer.
 *
 * Optimizes calls to 'is_null' using internal function
 */
class IsNullOptimizer extends IsTypeOptimizerAbstract
{
    protected function getType()
    {
        return 'IS_NULL';
    }
}
