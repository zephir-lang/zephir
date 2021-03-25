<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Bitwise;

class ShiftLeftOperator extends BitwiseBaseOperator
{
    protected $operator = '<<';

    protected $bitOperator = '<<';

    protected $zvalOperator = 'zephir_shift_left_function';
}
