<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

class NotOperator extends BaseOperator
{

	/**
	 * Compile expression
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{
		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		$leftExpr = new Expression($expression['left']);
		$leftExpr->setReadOnly($this->_readOnly);
		$left = $leftExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'bool':
				return new CompiledExpression('bool', '!' . $left->getCode(), $expression);
			default:
				throw new CompilerException("Unknown type: " . $left->getType(), $expression);
		}


	}

}