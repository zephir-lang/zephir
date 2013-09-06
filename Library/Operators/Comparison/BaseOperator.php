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
 * This is the base operator for comparison operators
 */
class ComparisonBaseOperator extends BaseOperator
{
	protected $_literalOnly = true;

	protected $_commutative = false;

	public function compile($expression, CompilationContext $compilationContext)
	{
		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		$leftExpr = new Expression($expression['left']);
		$leftExpr->setReadOnly($this->_readOnly);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$rightExpr->setReadOnly($this->_readOnly);
		$right = $rightExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
			case 'int':
			case 'char':
			case 'uchar':
				switch ($right->getType()) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);
					case 'char':
					case 'uchar':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' \'' . $right->getCode() . '\')', $expression);
					case 'double':
						return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' (int) ' . $right->getCode() . ')', $expression);
					default:
						throw new CompilerException("Error Processing Request", $expression);
				}
				break;
			case 'variable':
				$variable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);
				switch ($variable->getType()) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
					case 'char':
					case 'uchar':
						switch ($right->getType()) {
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);
							case 'char':
							case 'uchar':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' \'' . $right->getCode() . '\')', $expression);
							case 'bool':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode() . ')', $expression);
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
								switch ($variableRight->getType()) {
									case 'int':
									case 'uint':
									case 'long':
									case 'ulong':
									case 'char':
									case 'uchar':
										return new CompiledExpression('bool', '(' . $variable->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);
									case 'variable':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('bool', $this->_zvalLongOperator . '(' . $variableRight->getName() . ', ' . $variable->getName() . ')', $expression);
									default:
										throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
								}
								break;
							default:
								throw new CompilerException("Cannot compare variable: " . $variable->getType() . " with: " . $right->getType(), $expression);
						}
						break;
					case 'double':
						switch ($right->getType()) {
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);
							case 'bool':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode() . ')', $expression);
							case 'char':
							case 'uchar':
								return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' \'' . $right->getCode() . '\')', $expression);
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
								switch ($variableRight->getType()) {
									case 'int':
									case 'uint':
									case 'long':
									case 'ulong':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('bool', '(' . $variable->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);
									case 'double':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('bool', '(' . $variable->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);
									case 'variable':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('bool', 'ZEPHIR_IS_DOUBLE(' . $variableRight->getName() . ', ' . $variableLeft->getName() . ')', $expression);
									default:
										throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
								}
								break;
							default:
								throw new CompilerException("Cannot compare variable: " . $variable->getType() . " with: " . $right->getType(), $expression);
						}
						break;
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
							case 'uint':
							case 'long':
							case 'ulong':
								$compilationContext->headersManager->add('kernel/operators');
								if ($variable->isLocalOnly()) {
									return new CompiledExpression('bool', $this->_zvalLongOperator . '(&' . $variable->getName() . ', ' . $right->getCode() . ')', $expression['left']);
								} else {
									return new CompiledExpression('bool', $this->_zvalLongOperator . '(' . $variable->getName() . ', ' . $right->getCode() . ')', $expression['left']);
								}
								break;
							case 'bool':
								$compilationContext->headersManager->add('kernel/operators');
								if ($right->getCode() == 'true') {
									if ($variable->isLocalOnly()) {
										return new CompiledExpression('bool', 'ZEPHIR_IS_TRUE(&' . $variable->getName() . ')', $expression['left']);
									} else {
										return new CompiledExpression('bool', 'ZEPHIR_IS_TRUE(' . $variable->getName() . ')', $expression['left']);
									}
								} else {
									return new CompiledExpression('bool', 'ZEPHIR_IS_FALSE(' . $left->getCode() . ')', $expression['left']);
								}
								break;
							case 'null':
								$compilationContext->headersManager->add('kernel/operators');
								if ($variable->isLocalOnly()) {
									return new CompiledExpression('bool', '(Z_TYPE_P(&' . $variable->getName() . ') ' . $this->_operator . ' IS_NULL)', $expression['left']);
								} else {
									return new CompiledExpression('bool', '(Z_TYPE_P(' . $variable->getName() . ') ' . $this->_operator . ' IS_NULL)', $expression['left']);
								}
								break;
							case 'string':
								$compilationContext->headersManager->add('kernel/operators');
								if ($variable->isLocalOnly()) {
									return new CompiledExpression('bool', $this->_zvalStringOperator . '(&' . $variable->getName() . ', "' . $right->getCode() . '")', $expression['left']);
								} else {
									return new CompiledExpression('bool', $this->_zvalStringOperator . '(' . $variable->getName() . ', "' . $right->getCode() . '")', $expression['left']);
								}
								break;
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
								switch ($variableRight->getType()) {
									case 'int':
									case 'uint':
									case 'long':
									case 'ulong':
										$compilationContext->headersManager->add('kernel/operators');
										if ($variable->isLocalOnly()) {
											return new CompiledExpression('bool', $this->_zvalLongOperator . '(&' . $variable->getName() . ', ' . $variableRight->getName() . ')', $expression);
										} else {
											return new CompiledExpression('bool', $this->_zvalLongOperator . '(' . $variable->getName() . ', ' . $variableRight->getName() . ')', $expression);
										}
									case 'variable':
										$compilationContext->headersManager->add('kernel/operators');
										if ($variable->isLocalOnly()) {
											return new CompiledExpression('bool', $this->_zvalOperator . '(&' . $variable->getName() . ', ' . $variableRight->getName() . ')', $expression);
										} else {
											return new CompiledExpression('bool', $this->_zvalOperator . '(' . $variable->getName() . ', ' . $variableRight->getName() . ')', $expression);
										}
									default:
										throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
								}
								break;
							default:
								throw new CompilerException("Unknown type: " . $right->getType(), $expression['left']);
						}
						break;
					default:
						throw new CompilerException("Unknown type: " . $variable->getType(), $expression);
				}
				break;
			default:
				throw new CompilerException("Error Processing Request", $expression);
		}

	}
}