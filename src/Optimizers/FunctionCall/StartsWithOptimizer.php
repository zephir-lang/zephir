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

use function count;

/**
 * StartsWithOptimizer.
 *
 * Checks if a string starts with another
 */
class StartsWithOptimizer extends EndsWithOptimizer
{
    protected string $zephirMethod    = 'zephir_start_with';
    protected string $zephirMethodStr = 'zephir_start_with_str';
}
