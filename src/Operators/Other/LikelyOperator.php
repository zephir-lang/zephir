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

namespace Zephir\Operators\Other;

/**
 * Adds a branch prediction hint when evaluating an expression
 */
class LikelyOperator extends UnlikelyOperator
{
    protected string $operator     = 'likely';
    protected string $zephirMethod = 'EXPECTED';
}
