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
 * TrimOptimizer.
 *
 * Optimizes calls to 'trim' using internal function
 */
class TrimOptimizer extends UncamelizeOptimizer
{
    protected string $trimWhere    = 'ZEPHIR_TRIM_BOTH';
    protected string $warningName  = '';
    protected string $zephirMethod = 'zephir_fast_trim';

    /**
     * @param $parameters
     *
     * @return void
     */
    protected function checkParameters($parameters): void
    {
        // empty
    }
}
