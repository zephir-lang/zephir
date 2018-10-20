<?php

/**
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Operators\Comparison;

class NotEqualsOperator extends ComparisonBaseOperator
{
    protected $_commutative = true;

    protected $_inverse = true;

    protected $_operator = '!=';

    protected $_bitOperator = '!=';

    protected $_zvalOperator = '!ZEPHIR_IS_EQUAL';

    protected $_zvalLongOperator = '!ZEPHIR_IS_LONG';

    protected $_zvalLongNegOperator = '!ZEPHIR_IS_LONG';

    protected $_zvalDoubleOperator = '!ZEPHIR_IS_DOUBLE';

    protected $_zvalStringOperator = '!ZEPHIR_IS_STRING';

    protected $_zvalBoolOperator = '!ZEPHIR_IS_BOOL_VALUE';

    protected $_zvalBoolTrueOperator = '!ZEPHIR_IS_TRUE';

    protected $_zvalBoolFalseOperator = '!ZEPHIR_IS_FALSE';
}
