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
 * Optimizes calls to 'get_class_lower' using internal function
 */
class GetClassLowerOptimizer extends GetClassOptimizer
{
    protected bool $lower = true;
}
