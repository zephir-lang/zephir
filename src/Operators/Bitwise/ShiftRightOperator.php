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

class ShiftRightOperator extends ShiftBaseOperator
{
    protected string $bitOperator  = '>>';
    protected string $operator     = '>>';
    protected string $safeHelper   = 'zephir_safe_shift_right_long';
    protected string $zvalOperator = 'zephir_shift_right_function';

    protected function inlineShift(string $left, string $right): string
    {
        return '(' . $left . ' >> ' . $right . ')';
    }
}
