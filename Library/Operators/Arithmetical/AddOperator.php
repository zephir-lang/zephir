<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Arithmetical;

/**
 * AddOperator.
 *
 * Generates an arithmetical operation according to the operands
 */
class AddOperator extends ArithmeticalBaseOperator
{
    protected $operator = '+';

    protected $bitOperator = '|';

    protected $zvalOperator = 'zephir_add_function';
}
