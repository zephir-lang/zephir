<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
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
