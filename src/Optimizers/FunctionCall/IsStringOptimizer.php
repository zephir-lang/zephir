<?php

declare(strict_types=1);

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
 * IsStringOptimizer.
 *
 * Optimizes calls to 'is_string' using internal function
 */
class IsStringOptimizer extends IsTypeOptimizerAbstract
{
    protected function getType()
    {
        return 'IS_STRING';
    }
}
