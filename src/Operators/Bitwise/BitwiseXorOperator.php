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

namespace Zephir\Operators\Bitwise;

class BitwiseXorOperator extends BitwiseBaseOperator
{
    protected string $bitOperator  = '^';
    protected string $operator     = '^';
    protected string $zvalOperator = 'zephir_bitwise_xor_function';
}
