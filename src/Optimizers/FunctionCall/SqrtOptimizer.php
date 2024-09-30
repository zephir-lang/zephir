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

use Zephir\Optimizers\MathOptimizer;

/**
 * SqrtOptimizer.
 *
 * Optimizes calls to 'sqrt' using internal function
 */
class SqrtOptimizer extends MathOptimizer
{
    protected string $zephirMethod = 'sqrt';
}
