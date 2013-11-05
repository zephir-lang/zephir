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
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
				return new CompiledExpression('bool', '!' . $left->getCode(), $expression);
			case 'variable':
				$variable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);
				switch ($variable->getType()) {
					case 'bool':
					case 'int':
					case 'uint':
					case 'long':
						return new CompiledExpression('bool', '!' . $variable->getName(), $expression);
					case 'variable':
						return new CompiledExpression('bool', '!zephir_is_true(' . $variable->getName() . ')', $expression);
					default:
						throw new CompilerException("Unknown type: " . $variable->getType(), $expression);
				}
			default:
				throw new CompilerException("Unknown type: " . $left->getType(), $expression);
		}

	}

}
