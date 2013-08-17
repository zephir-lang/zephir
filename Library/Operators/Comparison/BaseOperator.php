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

/**
 * BaseOperator
 *
 * This is the base operator for commutative, associative and distributive
 * arithmetic operators
 */
class ComparisonBaseOperator extends BaseOperator
{

	public function compile($expression, CompilationContext $compilationContext)
	{
		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		$leftExpr = new Expression($expression['left']);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$right = $rightExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'variable':

				$variable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);

				switch ($variable->getType()) {
					case 'int':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression['left']);
					case 'bool':
						switch ($right->getType()) {
							case 'int':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression['left']);
							case 'bool':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode() . ')', $expression['left']);
							case 'null':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' 0)', $expression['left']);
							default:
								throw new CompilerException("Error Processing Request", $expression['left']);
						}
						break;
					case 'variable':
						switch ($right->getType()) {
							case 'int':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'ZEPHIR_IS_LONG(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression['left']);
							case 'bool':
								$compilationContext->headersManager->add('kernel/operators');
								if ($right->getCode() == 'true') {
									return new CompiledExpression('bool', 'ZEPHIR_IS_TRUE(' . $left->getCode() . ')', $expression['left']);
								} else {
									return new CompiledExpression('bool', 'ZEPHIR_IS_FALSE(' . $left->getCode() . ')', $expression['left']);
								}
							case 'null':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'Z_TYPE_P(' . $left->getCode() . ') ' . $this->_operator . ' IS_NULL', $expression['left']);
							case 'variable':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('bool', 'is_equals_operator(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression);
							default:
								throw new CompilerException("Error Processing Request", $expression['left']);
						}
						break;
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			case 'int':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);
					case 'double':
						return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' (int) ' . $right->getCode(), $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			default:
				throw new CompilerException("Error Processing Request", $expression);
		}

	}
}