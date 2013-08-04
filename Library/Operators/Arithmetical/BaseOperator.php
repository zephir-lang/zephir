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
 * This is the base operator for commutative, associative anddistributive
 * operators
 */
class BaseOperator
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
			case 'int':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);
					case 'double':
						return new CompiledExpression('double', '(double) ' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);
					case 'bool':
						return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode(), $expression);
					case 'variable':
						$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value'], $compilationContext, $expression);
						switch ($variableRight->getType()) {
							case 'int':
								return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);
							case 'bool':
								return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);
							case 'double':
								return new CompiledExpression('double', ' (double) ' . $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);
							case 'variable':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' zephir_get_intval(' . $variableRight->getName() . ')', $expression);
							default:
								throw new Exception("Cannot add variable('int') with variable('" . $variableRight->getType() . "')");
						}
					default:
						throw new Exception("Cannot add 'int' with '" . $right->getType() . "'");
				}
				break;
			case 'bool':
				switch ($right->getType()) {
					case 'int':
					case 'double':
						return new CompiledExpression('bool', $left->getBooleanCode() . ' ' . $this->_bitOperator . '((' . $right->getCode() . ') ? 1 : 0)', $expression);
					case 'bool':
						return new CompiledExpression('bool', $left->getBooleanCode() . ' ' . $this->_bitOperator . '' . $right->getBooleanCode(), $expression);
					default:
						throw new Exception("Cannot add 'bool' with '" . $right->getType() . "'");
				}
				break;
			case 'double':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('double', $left->getCode() . ' ' . $this->_operator . ' (double) (' . $right->getCode() . ')', $expression);
					case 'double':
						return new CompiledExpression('double', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);
					case 'bool':
						return new CompiledExpression('double', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode(), $expression);
					default:
						throw new CompilerException("Cannot add 'double' with '" . $right->getType() . "'", $expression);
				}
				break;
			case 'string':
				switch ($right->getType()) {
					default:
						throw new CompilerException("Add is not supported between strings", $expression);
				}
				break;
			case 'variable':

				$variableLeft = $compilationContext->symbolTable->getVariableForRead($left->resolve(null, $compilationContext), $compilationContext, $expression);

				switch ($variableLeft->getType()) {
					case 'int':
						switch ($right->getType()) {
							case 'int':
								return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value'], $compilationContext, $expression['right']);
								switch ($variableRight->getType()) {
									case 'int':
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);
									case 'bool':
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);
									case 'double':
										return new CompiledExpression('double', ' (double) ' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);
									case 'variable':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' zephir_get_intval(' . $variableRight->getName() . ')', $expression);
									default:
										throw new CompilerException("Cannot add variable('int') with variable('" . $variableRight->getType() . "')", $expression);
								}
							default:
								throw new CompilerException("Cannot add variable('int') with '" . $right->getType() . "'", $expression);
						}
						break;
					case 'bool':
						switch ($right->getType()) {
							case 'int':
								return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);
							case 'bool':
								return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_bitOperator . '' . $right->getBooleanCode(), $expression);
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value'], $compilationContext, $expression['right']);
								switch ($variableRight->getType()) {
									case 'int':
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);
									case 'bool':
										return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_bitOperator . '' . $variableRight->getName(), $expression);
									case 'variable':
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' zephir_get_intval(' . $variableRight->getName() . ')', $expression);
									default:
										throw new CompilerException("Cannot add variable('int') with variable('" . $variableRight->getType() . "')", $expression);
								}
							default:
								throw new CompilerException("Cannot add variable('int') with '" . $right->getType() . "'", $expression);
						}
					case 'double':
						switch ($right->getType()) {
							case 'int':
								return new CompiledExpression('double', $left->getCode() . ' ' . $this->_operator . ' (double) ' . $right->getCode(), $expression);
							case 'double':
								return new CompiledExpression('double', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);
							case 'bool':
								return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_bitOperator . '' . $right->getBooleanCode(), $expression);
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value'], $compilationContext, $expression['right']);
								switch ($variableRight->getType()) {
									case 'int':
										return new CompiledExpression('double', $variableLeft->getName() . ' ' . $this->_operator . '  (double) ' . $variableRight->getName(), $expression);
									case 'double':
										return new CompiledExpression('double', $variableLeft->getName() . ' ' . $this->_operator . '  ' . $variableRight->getName(), $expression);
									case 'bool':
										return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_bitOperator . '' . $variableRight->getName(), $expression);
									case 'variable':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' zephir_get_intval(' . $variableRight->getName() . ')', $expression);
									default:
										throw new Exception("Cannot add variable('double') with variable('" . $variableRight->getType() . "')");
								}
							default:
								throw new CompilerException("Cannot add variable('int') with '" . $right->getType() . "'", $expression);
						}
					case 'string':
						throw new CompilerException("Cannot add string variables'", $expression);
					case 'variable':

						switch ($right->getType()) {
							case 'int':
							case 'double':
								$compilationContext->headersManager->add('kernel/operators');
								$op = $this->_operator;
								$op1 = $variableLeft->getName();
								$op2 = $right->getCode();
								return new CompiledExpression('expr-variable', function($result) use ($op1, $op, $op2) {
									return 'ZVAL_DOUBLE(' . $result . ', zephir_get_doubleval(' . $op1 . ') ' . $op . ' ' . $op2 . ');';
								}, $expression);
								break;
							case 'variable':

								$variableRight = $compilationContext->symbolTable->getVariableForRead($right->resolve(null, $compilationContext), $compilationContext, $expression);

								switch ($variableRight->getType()) {
									case 'int':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('int', 'zephir_get_intval(' . $variableLeft->getName() . ') ' . $this->_operator . ' ' . $variableRight->getName(), $expression);
									case 'bool':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('int', 'zephir_get_intval(' . $variableLeft->getName() . ') ' . $this->_operator . ' ' . $variableRight->getName(), $expression);
									case 'variable':
										$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value'], $compilationContext, $expression);
										switch ($variableRight->getType()) {
											case 'int':
												return new CompiledExpression('double', $variableLeft->getName() . ' ' . $this->_operator . '  (double) ' . $variableRight->getName(), $expression);
											case 'double':
												return new CompiledExpression('double', $variableLeft->getName() . ' ' . $this->_operator . '  ' . $variableRight->getName(), $expression);
											case 'bool':
												return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_bitOperator . '' . $variableRight->getName(), $expression);
											case 'variable':
												$compilationContext->headersManager->add('kernel/operators');
												$op1 = $variableLeft->getName();
												$op2 = $variableRight->getName();
												return new CompiledExpression('expr-variable', function($result) use ($op1, $op2) {
													return 'zephir_add_function(' . $result . ', ' . $op1 . ', ' . $op2 . ');';
												}, $expression);
											default:
												throw new CompilerException("Cannot add variable('double') with variable('" . $variableRight->getType() . "')", $expression);
										}
									default:
										throw new CompilerException("Cannot add 'variable' with variable ('" . $variableRight->getType() . "')", $expression);
								}
							default:
								throw new CompilerException("Cannot add 'variable' with '" . $right->getType() . "'", $expression);
						}
					default:
						throw new CompilerException("Cannot add 'variable' with variable('" . $variableLeft->getType() . "')", $expression);
				}
				break;
			case 'expr-variable':
				return new CompiledExpression('variable', $left->resolve(null, $compilationContext), $expression);
			default:
				throw new CompilerException("Unsupported type: " . $left->getType(), $expression);
		}
	}
}