<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Operators\Comparison;

class GreaterOperator extends ComparisonBaseOperator
{

    protected $_operator = '>';

    protected $_bitOperator = '>';

    protected $_zvalOperator = 'ZEPHIR_GT';

    protected $_zvalLongOperator = 'ZEPHIR_GT_LONG';

    protected $_zvalLongNegOperator = 'ZEPHIR_LT_LONG';

    protected $_zvalDoubleOperator = 'ZEPHIR_GT_DOUBLE';

    protected $_zvalDoubleNegOperator = 'ZEPHIR_LT_DOUBLE';
}
