<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Bitwise;

class BitwiseAndOperator extends BitwiseBaseOperator
{
    protected string $operator = '&';

    protected string $bitOperator = '&';

    protected string $zvalOperator = 'zephir_bitwise_and_function';
}
