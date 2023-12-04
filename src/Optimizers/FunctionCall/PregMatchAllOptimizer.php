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
 * PregMatchAllOptimizer.
 *
 * Optimizes calls to 'preg_match_all' using internal function
 */
class PregMatchAllOptimizer extends PregMatchOptimizer
{
    public const GLOBAL_MATCH = 1;
}
