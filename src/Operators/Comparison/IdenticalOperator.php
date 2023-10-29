<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Comparison;

class IdenticalOperator extends ComparisonBaseOperator
{
    protected bool $commutative = true;

    protected string $operator = '==';

    protected string $bitOperator = '==';

    protected string $zvalOperator = 'ZEPHIR_IS_IDENTICAL';

    protected string $zvalLongOperator = 'ZEPHIR_IS_LONG_IDENTICAL';

    protected string $zvalLongNegOperator = 'ZEPHIR_IS_LONG_IDENTICAL';

    protected string $zvalStringOperator = 'ZEPHIR_IS_STRING_IDENTICAL';

    protected string $zvalBoolOperator = 'ZEPHIR_IS_BOOL_IDENTICAL';

    protected string $zvalBoolTrueOperator = 'ZEPHIR_IS_TRUE_IDENTICAL';

    protected string $zvalBoolFalseOperator = 'ZEPHIR_IS_FALSE_IDENTICAL';

    protected string $zvalNullOperator = 'ZEPHIR_IS_NULL';
}
