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

class NotEqualsOperator extends ComparisonBaseOperator
{
    protected string $bitOperator           = '!=';
    protected bool   $commutative           = true;
    protected bool   $inverse               = true;
    protected string $operator              = '!=';
    protected string $zvalBoolFalseOperator = '!ZEPHIR_IS_FALSE';
    protected string $zvalBoolOperator      = '!ZEPHIR_IS_BOOL_VALUE';
    protected string $zvalBoolTrueOperator  = '!ZEPHIR_IS_TRUE';
    protected string $zvalDoubleOperator    = '!ZEPHIR_IS_DOUBLE';
    protected string $zvalLongNegOperator   = '!ZEPHIR_IS_LONG';
    protected string $zvalLongOperator      = '!ZEPHIR_IS_LONG';
    protected string $zvalNullOperator      = '!ZEPHIR_IS_NULL';
    protected string $zvalOperator          = '!ZEPHIR_IS_EQUAL';
    protected string $zvalStringOperator    = '!ZEPHIR_IS_STRING';
}
