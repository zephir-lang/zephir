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
 * SinOptimizer.
 *
 * Optimizes calls to 'sin' using internal function
 */
class SinOptimizer extends MathOptimizer
{
    protected string $zephirMethod = 'sin';
}
