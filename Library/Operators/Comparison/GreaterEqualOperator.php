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

class GreaterEqualOperator extends ComparisonBaseOperator
{
    protected $_operator = '>=';

    protected $_bitOperator = '>=';

    protected $_zvalOperator = 'ZEPHIR_GE';

    protected $_zvalLongOperator = 'ZEPHIR_GE_LONG';

    protected $_zvalLongNegOperator = 'ZEPHIR_LE_LONG';

    protected $_zvalDoubleOperator = '!ZEPHIR_LT_DOUBLE';

    protected $_zvalDoubleNegOperator = '!ZEPHIR_GT_DOUBLE';
}
