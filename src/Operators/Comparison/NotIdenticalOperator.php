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

namespace Zephir\Operators\Comparison;

class NotIdenticalOperator extends ComparisonBaseOperator
{
    protected string $bitOperator           = '!=';
    protected bool   $commutative           = true;
    protected bool   $inverse               = true;
    protected string $operator              = '!=';
    protected string $zvalBoolFalseOperator = '!ZEPHIR_IS_FALSE_IDENTICAL';
    protected string $zvalBoolOperator      = '!ZEPHIR_IS_BOOL_IDENTICAL';
    protected string $zvalBoolTrueOperator  = '!ZEPHIR_IS_TRUE_IDENTICAL';
    protected string $zvalLongNegOperator   = '!ZEPHIR_IS_LONG_IDENTICAL';
    protected string $zvalLongOperator      = '!ZEPHIR_IS_LONG_IDENTICAL';
    protected string $zvalNullOperator      = '!ZEPHIR_IS_NULL';
    protected string $zvalOperator          = '!ZEPHIR_IS_IDENTICAL';
    protected string $zvalStringOperator    = '!ZEPHIR_IS_STRING_IDENTICAL';
}
