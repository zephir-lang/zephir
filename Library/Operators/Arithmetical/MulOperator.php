<?php

/**
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Operators\Arithmetical;

/**
 * MulOperator
 *
 * Generates an arithmetical operation according to the operands
 */
class MulOperator extends ArithmeticalBaseOperator
{
    protected $_operator = '*';

    protected $_bitOperator = '+';

    protected $_zvalOperator = 'mul_function';
}
