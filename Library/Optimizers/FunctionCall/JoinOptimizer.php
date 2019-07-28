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
 * @see http://php.net/manual/en/function.join.php
 *
 * Optimizes calls to 'join' using ImplodeOptimizer via it is an alias
 */
class JoinOptimizer extends ImplodeOptimizer
{
}
