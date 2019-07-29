<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Optimizers\IsTypeOptimizerAbstract;

/**
 * IsObjectOptimizer.
 *
 * Optimizes calls to 'is_array' using internal function
 */
class IsObjectOptimizer extends IsTypeOptimizerAbstract
{
    protected function getType()
    {
        return 'IS_OBJECT';
    }
}
