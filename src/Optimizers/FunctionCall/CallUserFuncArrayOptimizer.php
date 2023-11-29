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
 * CallUserFuncArrayOptimizer.
 *
 * Optimizer for 'call_user_func_array'
 */
class CallUserFuncArrayOptimizer extends CallUserFuncOptimizer
{
    protected int    $parameterCount = 2;
    protected string $zephirMethod   = 'ZEPHIR_CALL_USER_FUNC_ARRAY';

    /**
     * @param string $symbol
     * @param array  $resolvedParams
     *
     * @return string
     */
    protected function getOutput(string $symbol, array $resolvedParams): string
    {
        return $this->zephirMethod
            . '(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');'
        ;
    }
}
