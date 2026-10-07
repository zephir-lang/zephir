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

namespace Zephir\Operators\Arithmetical;

use Zephir\Types\Types;

/**
 * Generates an arithmetical operation according to the operands
 */
class AddOperator extends ArithmeticalBaseOperator
{
    /**
     * `+` of two arrays is their union.
     */
    protected array $zvalResultTypes = [Types::T_LONG, Types::T_DOUBLE, Types::T_ARRAY, Types::T_OBJECT];

    protected string $operator     = '+';
    protected string $zvalOperator = 'zephir_add_function';
}
