<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

/**
 * PregMatchAllOptimizer.
 *
 * Optimizes calls to 'preg_match_all' using internal function
 */
class PregMatchAllOptimizer extends PregMatchOptimizer
{
    const GLOBAL_MATCH = 1;
}
