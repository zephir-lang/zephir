<?php

class AddOperator
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
						return new CompiledExpression('int', $left->getCode() . ' + ' . $right->getCode());
					case 'double':
						return new CompiledExpression('double', '(double) ' . $left->getCode() . ' + ' . $right->getCode());
					case 'bool':
						return new CompiledExpression('int', $left->getCode() . ' + ' . $right->getBooleanCode());
					case 'variable':
						$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);
						switch ($variableRight->getType()) {
							case 'int':
								return new CompiledExpression('int', $left->getCode() . ' + ' . $variableRight->getName());
							case 'bool':
								return new CompiledExpression('int', $left->getCode() . ' + ' . $variableRight->getName());
							case 'double':
								return new CompiledExpression('double', ' (double) ' . $left->getCode() . ' + ' . $variableRight->getName());
							case 'variable':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('int', $left->getCode() . ' + zephir_get_intval(' . $variableRight->getName() . ')');
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
						return new CompiledExpression('bool', $left->getBooleanCode() . ' | ((' . $right->getCode() . ') ? 1 : 0)');
					case 'bool':
						return new CompiledExpression('bool', $left->getBooleanCode() . ' | ' . $right->getBooleanCode());
					default:
						throw new Exception("Cannot add 'bool' with '" . $right->getType() . "'");
				}
				break;
			case 'double':
				switch ($right->getType()) {
					case 'int':
						return new CompiledExpression('double', $left->getCode() . ' + (double) (' . $right->getCode() . ')');
					case 'double':
						return new CompiledExpression('double', $left->getCode() . ' + ' . $right->getCode());
					case 'bool':
						return new CompiledExpression('double', $left->getCode() . ' + ' . $right->getBooleanCode());
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
								return new CompiledExpression('int', $left->getCode() . ' + ' . $right->getCode());
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);
								switch ($variableRight->getType()) {
									case 'int':
										return new CompiledExpression('int', $variableLeft->getName() . ' + ' . $variableRight->getName());
									case 'bool':
										return new CompiledExpression('int', $variableLeft->getName() . ' + ' . $variableRight->getName());
									case 'double':
										return new CompiledExpression('double', ' (double) ' . $variableLeft->getName() . ' + ' . $variableRight->getName());
									case 'variable':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('int', $variableLeft->getName() . ' + zephir_get_intval(' . $variableRight->getName() . ')');
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
								return new CompiledExpression('bool', $left->getCode() . ' + ' . $right->getCode());
							case 'bool':
								return new CompiledExpression('bool', $left->getCode() . ' | ' . $right->getBooleanCode());
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);
								switch ($variableRight->getType()) {
									case 'int':
										return new CompiledExpression('int', $variableLeft->getName() . ' + ' . $variableRight->getName());
									case 'bool':
										return new CompiledExpression('bool', $variableLeft->getName() . ' | ' . $variableRight->getName());
									case 'variable':
										return new CompiledExpression('int', $variableLeft->getName() . ' + zephir_get_intval(' . $variableRight->getName() . ')');
									default:
										throw new Exception("Cannot add variable('int') with variable('" . $variableRight->getType() . "')");
								}
							default:
								throw new Exception("Cannot add variable('int') with '" . $right->getType() . "'");
						}
					case 'double':
						switch ($right->getType()) {
							case 'int':
								return new CompiledExpression('double', $left->getCode() . ' + (double) ' . $right->getCode());
							case 'double':
								return new CompiledExpression('double', $left->getCode() . ' + ' . $right->getCode());
							case 'bool':
								return new CompiledExpression('bool', $left->getCode() . ' | ' . $right->getBooleanCode());
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);
								switch ($variableRight->getType()) {
									case 'int':
										return new CompiledExpression('double', $variableLeft->getName() . ' +  (double) ' . $variableRight->getName());
									case 'double':
										return new CompiledExpression('double', $variableLeft->getName() . ' +  ' . $variableRight->getName());
									case 'bool':
										return new CompiledExpression('bool', $variableLeft->getName() . ' | ' . $variableRight->getName());
									case 'variable':
										$compilationContext->headersManager->add('kernel/operators');
										return new CompiledExpression('int', $variableLeft->getName() . ' + zephir_get_intval(' . $variableRight->getName() . ')');
									default:
										throw new Exception("Cannot add variable('double') with variable('" . $variableRight->getType() . "')");
								}
							default:
								throw new Exception("Cannot add variable('int') with '" . $right->getType() . "'");
						}
					case 'string':
						throw new Exception("Cannot add string variables'");
					case 'variable':

						$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);

						switch ($variableRight->getType()) {
							case 'int':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('int', 'zephir_get_intval(' . $variableLeft->getName() . ') + ' . $variableRight->getName());
							case 'bool':
								$compilationContext->headersManager->add('kernel/operators');
								return new CompiledExpression('int', 'zephir_get_intval(' . $variableLeft->getName() . ') + ' . $variableRight->getName());
							case 'variable':
								$variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value']);
								switch ($variableRight->getType()) {
									case 'int':
										return new CompiledExpression('double', $variableLeft->getName() . ' +  (double) ' . $variableRight->getName());
									case 'double':
										return new CompiledExpression('double', $variableLeft->getName() . ' +  ' . $variableRight->getName());
									case 'bool':
										return new CompiledExpression('bool', $variableLeft->getName() . ' | ' . $variableRight->getName());
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
						throw new Exception("Cannot add 'variable' with '" . $variableLeft->getType() . "'");
				}
				break;
			default:
				throw new Exception("Unsupported type " . $left->getType());
		}
	}
}