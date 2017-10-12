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

namespace Zephir\Operators\Arithmetical;

/**
 * AddOperator
 *
 * Generates an arithmetical operation according to the operands
 */
class AddOperator extends ArithmeticalBaseOperator
{
    protected $_operator = '+';

    protected $_bitOperator = '|';

    protected $_zvalOperator = 'zephir_add_function';
}
