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

/**
 * EndsWithOptimizer.
 *
 * Like 'strpos' but it returns a boolean value
 */
class EndsWithOptimizer extends StartsWithOptimizer
{
    protected string $zephirMethodStr = 'zephir_end_with_str';
    protected string $zephirMethod    = 'zephir_end_with';
}
