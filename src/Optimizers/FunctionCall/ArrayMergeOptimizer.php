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

use function sprintf;

/**
 * ArrayMergeOptimizer.
 *
 * Optimizes calls to 'array_merge' using internal function
 */
class ArrayMergeOptimizer extends ArrayKeysOptimizer
{
    protected int $parameterCount = 2;

    /**
     * @param string $symbol
     * @param array  $resolvedParams
     *
     * @return string
     */
    protected function getOutput(string $symbol, $resolvedParams): string
    {
        return sprintf(
            'zephir_fast_array_merge(%s, %s, %s);',
            $symbol,
            $resolvedParams[0],
            $resolvedParams[1]
        );
    }
}
