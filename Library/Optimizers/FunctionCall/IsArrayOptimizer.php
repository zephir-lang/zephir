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

use Zephir\Optimizers\IsTypeOptimizerAbstract;

/**
 * IsArrayOptimizer.
 *
 * Optimizes calls to 'is_array' using internal function
 */
class IsArrayOptimizer extends IsTypeOptimizerAbstract
{
    protected function getType()
    {
        return 'IS_ARRAY';
    }
}
