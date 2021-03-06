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

use Zephir\Optimizers\MathOptimizer;

/**
 * ACosOptimizer.
 *
 * Optimizes calls to 'acos' using internal function
 */
class ACosOptimizer extends MathOptimizer
{
    public function getFunctionName()
    {
        return 'acos';
    }
}
