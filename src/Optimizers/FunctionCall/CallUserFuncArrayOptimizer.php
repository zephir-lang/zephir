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

use Zephir\CompilationContext;

use function array_map;
use function in_array;

/**
 * Optimizer for 'call_user_func_array', the callback resolved as for
 * call_user_func() (#2824).
 */
class CallUserFuncArrayOptimizer extends CallUserFuncOptimizer
{
    /**
     * `callback` and `args`, by position or by name, in either order.
     */
    protected function acceptsArguments(array $parameters): bool
    {
        return in_array(
            array_map(fn (array $parameter) => $parameter['name'] ?? null, $parameters),
            [[null, null], [null, 'args'], ['callback', 'args'], ['args', 'callback']],
            true
        );
    }

    protected function getOutput(
        string $symbol,
        array $resolvedParams,
        array $names,
        CompilationContext $context
    ): string {
        [$callback, $arguments] = 'args' === $names[0]
            ? [$resolvedParams[1], $resolvedParams[0]]
            : [$resolvedParams[0], $resolvedParams[1]];

        return 'ZEPHIR_CALL_USER_FUNC_ARRAY(' . $symbol . ', ' . $callback . ', ' . $arguments . ');';
    }
}
