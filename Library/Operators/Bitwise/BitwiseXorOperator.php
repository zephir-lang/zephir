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

namespace Zephir\Operators\Bitwise;

class BitwiseXorOperator extends BitwiseBaseOperator
{
    protected $_operator = '^';

    protected $_bitOperator = '^';

    protected $_zvalOperator = 'zephir_bitwise_xor_function';
}
