<?php

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

		//print_r($expression);

		$leftExpr = new Expression($expression['left']);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$right = $rightExpr->compile($compilationContext);

		switch ($left->getType()) {
			case 'int':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode());
					case 'double':
						return new CompiledExpression('double', '(double) ' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode());
					case 'bool':
						return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode());
					case 'variable':
						$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);
						switch ($variableRight->getType()) {
							case 'int':
								return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName());
							case 'bool':
								return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName());
							case 'double':
								return new CompiledExpression('double', ' (double) ' . $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName());
							case 'variable':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' zephir_get_intval(' . $variableRight->getName() . ')');
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
						return new CompiledExpression('bool', $left->getBooleanCode() . ' & ((' . $right->getCode() . ') ? 1 : 0)');
					case 'bool':
						return new CompiledExpression('bool', $left->getBooleanCode() . ' & ' . $right->getBooleanCode());
					default:
						throw new Exception("Cannot add 'bool' with '" . $right->getType() . "'");
				}
				break;
			case 'double':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('double', $left->getCode() . ' ' . $this->_operator . ' (double) (' . $right->getCode() . ')');
					case 'double':
						return new CompiledExpression('double', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode());
					case 'bool':
						return new CompiledExpression('double', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode());
					default:
						throw new Exception("Cannot add 'double' with '" . $right->getType() . "'");
				}
				break;
			case 'string':
				switch ($right->getType()) {
					default:
						throw new Exception("Add is not supported with strings");
				}
				break;
			case 'variable':

				$variableLeft = $compilationContext->symbolTable->getVariableForRead($left->resolve(null, $compilationContext));

				switch ($variableLeft->getType()) {
					case 'int':
						switch ($right->getType()) {
							case 'int':
								return new CompiledExpression('int', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode());
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);
								switch ($variableRight->getType()) {
									case 'int':
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName());
									case 'bool':
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName());
									case 'double':
										return new CompiledExpression('double', ' (double) ' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName());
									case 'variable':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' zephir_get_intval(' . $variableRight->getName() . ')');
									default:
										throw new Exception("Cannot add variable('int') with variable('" . $variableRight->getType() . "')");
								}
							default:
								throw new Exception("Cannot add variable('int') with '" . $right->getType() . "'");
						}
						break;
					case 'bool':
						switch ($right->getType()) {
							case 'int':
								return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode());
							case 'bool':
								return new CompiledExpression('bool', $left->getCode() . ' & ' . $right->getBooleanCode());
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);
								switch ($variableRight->getType()) {
									case 'int':
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName());
									case 'bool':
										return new CompiledExpression('bool', $variableLeft->getName() . ' & ' . $variableRight->getName());
									case 'variable':
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' zephir_get_intval(' . $variableRight->getName() . ')');
									default:
										throw new Exception("Cannot add variable('int') with variable('" . $variableRight->getType() . "')");
								}
							default:
								throw new Exception("Cannot add variable('int') with '" . $right->getType() . "'");
						}
					case 'double':
						switch ($right->getType()) {
							case 'int':
								return new CompiledExpression('double', $left->getCode() . ' ' . $this->_operator . ' (double) ' . $right->getCode());
							case 'double':
								return new CompiledExpression('double', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode());
							case 'bool':
								return new CompiledExpression('bool', $left->getCode() . ' & ' . $right->getBooleanCode());
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);
								switch ($variableRight->getType()) {
									case 'int':
										return new CompiledExpression('double', $variableLeft->getName() . ' ' . $this->_operator . '  (double) ' . $variableRight->getName());
									case 'double':
										return new CompiledExpression('double', $variableLeft->getName() . ' ' . $this->_operator . '  ' . $variableRight->getName());
									case 'bool':
										return new CompiledExpression('bool', $variableLeft->getName() . ' & ' . $variableRight->getName());
									case 'variable':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('int', $variableLeft->getName() . ' ' . $this->_operator . ' zephir_get_intval(' . $variableRight->getName() . ')');
									default:
										throw new Exception("Cannot add variable('double') with variable('" . $variableRight->getType() . "')");
								}
							default:
								throw new Exception("Cannot add variable('int') with '" . $right->getType() . "'");
						}
					case 'string':
						throw new Exception("Cannot add string variables'");
					case 'variable':

						switch ($right->getType()) {
							case 'int':
								$compilationContext->headersManager->add('kernel/operators');
								$op = $this->_operator;
								$op1 = $variableLeft->getName();
								$op2 = $right->getCode();
								return new CompiledExpression('variable', function($result) use ($op1, $op, $op2) {
									return 'ZVAL_DOUBLE(' . $result . ', zephir_get_intval(' . $op1 . ') ' . $op . ' ' . $op2 . ');';
								});
								break;
							case 'variable':

								$variableRight = $compilationContext->symbolTable->getVariableForRead($right->resolve(null, $compilationContext));

								switch ($variableRight->getType()) {
									case 'int':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('int', 'zephir_get_intval(' . $variableLeft->getName() . ') ' . $this->_operator . ' ' . $variableRight->getName());
									case 'bool':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('int', 'zephir_get_intval(' . $variableLeft->getName() . ') ' . $this->_operator . ' ' . $variableRight->getName());
									case 'variable':
										$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);
										switch ($variableRight->getType()) {
											case 'int':
												return new CompiledExpression('double', $variableLeft->getName() . ' ' . $this->_operator . '  (double) ' . $variableRight->getName());
											case 'double':
												return new CompiledExpression('double', $variableLeft->getName() . ' ' . $this->_operator . '  ' . $variableRight->getName());
											case 'bool':
												return new CompiledExpression('bool', $variableLeft->getName() . ' & ' . $variableRight->getName());
											case 'variable':
												$compilationContext->headersManager->add('kernel/operators');
												$op1 = $variableLeft->getName();
												$op2 = $variableRight->getName();
												return new CompiledExpression('variable', function($result) use ($op1, $op2) {
													return 'zephir_add_function(' . $result . ', ' . $op1 . ', ' . $op2 . ');';
												});
											default:
												throw new Exception("Cannot add variable('double') with variable('" . $variableRight->getType() . "')");
										}
									default:
										throw new Exception("Cannot add 'variable' with variable ('" . $variableRight->getType() . "')");
								}
							default:
								throw new Exception("Cannot add 'variable' with '" . $right->getType() . "'");
						}
					default:
						throw new Exception("Cannot add 'variable' with variable('" . $variableLeft->getType() . "')");
				}
				break;
			default:
				throw new Exception("Unsupported type " . $left->getType());
		}
	}
}