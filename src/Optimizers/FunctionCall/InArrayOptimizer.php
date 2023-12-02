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
 * InArrayOptimizer.
 *
 * Optimizes calls to 'in_array' using internal function
 */
class InArrayOptimizer extends ArrayKeyExistsOptimizer
{
    /**
     * @param array $resolvedParams
     *
     * @return string
     */
    protected function getCode(array $resolvedParams): string
    {
        return 'zephir_fast_in_array('
            . $resolvedParams[0] . ', ' . $resolvedParams[1] . ')';
    }
}
