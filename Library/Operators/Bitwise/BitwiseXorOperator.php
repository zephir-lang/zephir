<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Operators\Bitwise;

class BitwiseXorOperator extends BitwiseBaseOperator
{
    protected $_operator = '^';

    protected $_bitOperator = '^';

    protected $_zvalOperator = 'zephir_bitwise_xor_function';
}
