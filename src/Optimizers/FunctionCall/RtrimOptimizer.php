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
 * RtrimOptimizer.
 *
 * Optimizes calls to 'rtrim' using internal function
 */
class RtrimOptimizer extends TrimOptimizer
{
    protected string $trimWhere = ', ZEPHIR_TRIM_RIGHT';
}
