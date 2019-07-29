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
 * SubOperator.
 *
 * Generates an arithmetical operation according to the operands
 */
class SubOperator extends ArithmeticalBaseOperator
{
    protected $operator = '-';

    protected $bitOperator = '&';

    protected $zvalOperator = 'zephir_sub_function';
}
