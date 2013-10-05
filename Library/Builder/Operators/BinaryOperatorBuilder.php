<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
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

/**
 * BinaryOperatorBuilder
 *
 * Allows to manually build a binary operator AST node
 */
class BinaryOperatorBuilder
{

	protected $_operator;

	public function __construct($operator, $leftExpression, $rightExpression)
	{
		$this->_operator = $operator;
		$this->_leftExpression = $leftExpression;
		$this->_rightExpression = $rightExpression;
	}

	public function get()
	{
		return array(
			'type' => $this->_operator,
			'left' => $this->_leftExpression->get(),
			'right' => $this->_rightExpression->get()
		);
	}

}
