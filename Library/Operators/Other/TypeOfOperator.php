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

/**
 * TypeOf
 *
 * Obtains the type of a dynamic variable
 */
class TypeOfOperator extends BaseOperator
{

	/**
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{

		if (!isset($expression['left'])) {
			throw new CompilerException("Invalid 'left' operand for 'typeof' expression", $expression['left']);
		}

		$builder = new FunctionCallBuilder('gettype', array($expression['left']));

		$expression = new Expression($builder->get());

		return $expression->compile($compilationContext);
	}

}
