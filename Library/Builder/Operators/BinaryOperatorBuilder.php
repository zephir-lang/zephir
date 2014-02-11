<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

namespace Zephir\Builder\Operators;

/**
 * BinaryOperatorBuilder
 *
 * Allows to manually build a binary operator AST node
 */
class BinaryOperatorBuilder
{

    protected $_operator;

    protected $_file;

    protected $_line;

    protected $_char;

    public function __construct($operator, $leftExpression, $rightExpression, $file = null, $line = 0, $char = 0)
    {
        $this->_operator = $operator;
        $this->_leftExpression = $leftExpression;
        $this->_rightExpression = $rightExpression;
        $this->_file = $file;
        $this->_line = $line;
        $this->_char = $char;
    }

    public function get()
    {
        return array(
            'type'       => $this->_operator,
            'left'       => $this->_leftExpression->get(),
            'right'      => $this->_rightExpression->get(),
            'file'       => $this->_file,
            'line'       => $this->_line,
            'char'       => $this->_char
        );
    }
}
