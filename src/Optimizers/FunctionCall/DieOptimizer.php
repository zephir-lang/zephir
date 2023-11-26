<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

/**
 * DieOptimizer.
 *
 * Optimizes calls to 'die' using internal function
 * Alias of exit
 */
class DieOptimizer extends ExitOptimizer
{
}
