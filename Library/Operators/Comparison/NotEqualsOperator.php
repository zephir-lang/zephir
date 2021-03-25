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

class NotEqualsOperator extends ComparisonBaseOperator
{
    protected $commutative = true;

    protected $inverse = true;

    protected $operator = '!=';

    protected $bitOperator = '!=';

    protected $zvalOperator = '!ZEPHIR_IS_EQUAL';

    protected $zvalLongOperator = '!ZEPHIR_IS_LONG';

    protected $zvalLongNegOperator = '!ZEPHIR_IS_LONG';

    protected $zvalDoubleOperator = '!ZEPHIR_IS_DOUBLE';

    protected $zvalStringOperator = '!ZEPHIR_IS_STRING';

    protected $zvalBoolOperator = '!ZEPHIR_IS_BOOL_VALUE';

    protected $zvalBoolTrueOperator = '!ZEPHIR_IS_TRUE';

    protected $zvalBoolFalseOperator = '!ZEPHIR_IS_FALSE';
}
