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
 * MethodCall
 *
 * Call methods in a non-static context
 */
class MethodCall extends Call
{

	/**
	 * Function is called using a normal method name
	 */
	const CALL_NORMAL = 1;

	/**
	 * Function is called using a dynamic variable as method name
	 */
	const CALL_DYNAMIC = 2;

	/**
	 * Function is called using a dynamic string as method name
	 */
	const CALL_DYNAMIC_STRING = 3;

	/**
	 *
	 *
	 * @param array $expr
	 * @param CompilationContext $compilationContext
	 */
	public function compile(Expression $expr, CompilationContext $compilationContext)
	{

		$expression = $expr->getExpression();

		$exprVariable = new Expression($expression['variable']);
		$exprVariable->setReadOnly(true);
		$exprCompiledVariable = $exprVariable->compile($compilationContext);

		switch ($exprCompiledVariable->getType()) {
			case 'variable':
				$variableVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression);
				switch ($variableVariable->getType()) {
					case 'variable':
						break;
					default:
						throw new CompilerException("Methods cannot be called on variable type: " . $variableVariable->getType(), $expression);
				}
				break;
			default:
				throw new CompiledException("Cannot use expression: " . $exprCompiledVariable->getType() . " as method caller", $expression['variable']);
		}

		$codePrinter = $compilationContext->codePrinter;

		$type = $expression['call-type'];

		/**
		 * In normal method calls and dynamic string method calls we just use the name given by the user
		 */
		if ($type == self::CALL_NORMAL || $type == self::CALL_DYNAMIC_STRING) {
			$methodName = strtolower($expression['name']);
		} else {
			$variableMethod = $compilationContext->symbolTable->getVariableForRead($expression['name'], $compilationContext, $expression);
			if ($variableMethod->getType() != 'variable' && $variableMethod->getType() != 'string') {
				throw new CompilerException("Cannot use variable type: " . $variableMethod->getType() . " as dynamic method name", $expression);
			}
		}

		$symbolVariable = null;

		/**
		 * Create temporary variable if needed
		 */
		$mustInit = false;
		$isExpecting = $expr->isExpectingReturn();
		if ($isExpecting) {
			$symbolVariable = $expr->getExpectingVariable();
			if (is_object($symbolVariable)) {
				$readDetector = new ReadDetector($expression);
				if ($readDetector->detect($symbolVariable->getName(), $expression)) {
					$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
				} else {
					$mustInit = true;
				}
			} else {
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
			}
		}

		/**
		 * Method calls only return zvals so we need to validate the target variable is also a zval
		 */
		if ($isExpecting) {

			if ($symbolVariable->getType() != 'variable') {
				throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
			}

			/**
			 * At this point, we don't know the exact dynamic type returned by the method call
			 */
			$symbolVariable->setDynamicTypes('undefined');
		}

		/**
		 * Try to check if the method exist in the callee, only when method call is self::CALL_NORMAL
		 */
		if ($type == self::CALL_NORMAL) {

			if ($variableVariable->getRealName() == 'this') {

				$classDefinition = $compilationContext->classDefinition;
				if (!$classDefinition->hasMethod($methodName)) {
					throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not implement method: '" . $expression['name'] . "'", $expression);
				}

				$method = $classDefinition->getMethod($methodName);

				/**
				 * Private methods must be called in their declaration scope
				 */
				if ($method->isPrivate()) {
					if ($method->getClassDefinition() != $classDefinition) {
						throw new CompilerException("Cannot call private method '" . $expression['name'] . "' out of its scope", $expression);
					}
				}

				/**
				 * Try to produce an exception if method is called with a wrong number
				 * of parameters
				 */
				if (isset($expression['parameters'])) {
					$callNumberParameters = count($expression['parameters']);
				} else {
					$callNumberParameters = 0;
				}

				$classMethod = $classDefinition->getMethod($methodName);
				$expectedNumberParameters = $classMethod->getNumberOfRequiredParameters();

				if (!$expectedNumberParameters && $callNumberParameters > 0) {
					$numberParameters = $classMethod->getNumberOfParameters();
					if ($callNumberParameters > $numberParameters) {
						throw new CompilerException("Method '" . $classDefinition->getCompleteName() . "::" . $expression['name'] . "' called with a wrong number of parameters, the method has: " . $expectedNumberParameters . ", passed: " . $callNumberParameters, $expression);
					}
				}

				if ($callNumberParameters < $expectedNumberParameters) {
					throw new CompilerException("Method '" . $classDefinition->getCompleteName() . "::" . $expression['name'] . "' called with a wrong number of parameters, the method has: " . $expectedNumberParameters . ", passed: " . $callNumberParameters, $expression);
				}

			} else {

				/**
				 * Variables whose dynamic type is 'object' can be used
				 * to determine method existance in compile time
				 */
				if ($variableVariable->hasAnyDynamicType('object')) {

					$classTypes = $variableVariable->getClassTypes();
					if (count($classTypes)) {

						//var_dump($classTypes);
						//var_dump($variableVariable->getOriginal());

						$numberImplemented = 0;
						$compiler = $compilationContext->compiler;
						foreach ($classTypes as $classType) {

							if ($compiler->isClass($classType) || $compiler->isInterface($classType) ||
								$compiler->isInternalClass($classType) || $compiler->isInternalInterface($classType)) {

								if ($compiler->isClass($classType) || $compiler->isInterface($classType)) {
									$classDefinition = $compiler->getClassDefinition($classType);
								} else {
									$classDefinition = $compiler->getInternalClassDefinition($classType);
								}

								if (!$classDefinition) {
									throw new CompilerException("Cannot locate class definition for class " . $classType, $expression);
								}

								if (!$classDefinition->hasMethod($expression['name'])) {
									if (count($classTypes) == 1) {
										throw new CompilerException("Class '" . $classType . "' does not implement method: '" . $expression['name'] . "'", $expression);
									}
									continue;
								}

								$method = $classDefinition->getMethod($methodName);

								/**
								 * Private methods must be called in their declaration scope
								 */
								if ($method->isPrivate()) {
									if ($method->getClassDefinition() != $classDefinition) {
										throw new CompilerException("Cannot call private method '" . $expression['name'] . "' out of its scope", $expression);
									}
								}

								if ($method->isProtected() && $method->getClassDefinition() != $classDefinition && $method->getClassDefinition() != $classDefinition->getExtendsClass()) {
									throw new CompilerException("Cannot call protected method '" . $expression['name'] . "' out of its scope", $expression);
								}

								/**
								 * Try to produce an exception if method is called with a wrong number of parameters
								 * We only check extension parameters if methods are extension methods
								 * Internal methods may have invalid Reflection information
								 */
								if ($method instanceof ClassMethod) {

									if (isset($expression['parameters'])) {
										$callNumberParameters = count($expression['parameters']);
									} else {
										$callNumberParameters = 0;
									}

									$classMethod = $classDefinition->getMethod($methodName);
									$expectedNumberParameters = $classMethod->getNumberOfRequiredParameters();

									if (!$expectedNumberParameters && $callNumberParameters > 0) {
										$numberParameters = $classMethod->getNumberOfParameters();
										if ($callNumberParameters > $numberParameters) {
											$className = $classDefinition->getCompleteName();
											throw new CompilerException("Method '" . $className . "::" . $expression['name'] . "' called with a wrong number of parameters, the method has: " . $expectedNumberParameters . ", passed: " . $callNumberParameters, $expression);
										}
									}

									if ($callNumberParameters < $expectedNumberParameters) {
										throw new CompilerException("Method '" . $classDefinition->getCompleteName() . "::" . $expression['name'] . "' called with a wrong number of parameters, the method has: " . $expectedNumberParameters . ", passed: " . $callNumberParameters, $expression);
									}
								}

								/**
								 * The method is checked in the first class that implements the method
								 * We could probably have collisions here
								 */
								$numberImplemented++;
								break;

							} else {
								$numberImplemented++;
								$compilationContext->logger->warning("Class \"" . $classType . "\" does not exist at compile time", "nonexistent-class", $expression);
							}

						}

						if ($numberImplemented == 0) {
							if (count($classTypes) > 1) {
								throw new CompilerException("None of classes: '" . join(' or ', $classTypes) . "' implement method: '" . $expression['name'] . "'", $expression);
							} else {
								throw new CompilerException("Class '" . $classTypes[0] . "' does not implement method: '" . $expression['name'] . "'", $expression);
							}
						}
					}
				}
			}
		}

		/**
		 * Transfer the return type-hint to the returned variable
		 */
		if ($isExpecting) {
			if (isset($method)) {
				if ($method instanceof ClassMethod) {

					if ($method->isVoid()) {
						throw new CompilerException("Method '" . $classDefinition->getCompleteName() . "::" . $expression['name'] . "' is marked as 'void' and it does not return anything", $expression);
					}

					$returnClassTypes = $method->getReturnClassTypes();

					if ($returnClassTypes !== null) {
						$symbolVariable->setDynamicTypes('object');
						$symbolVariable->setClassTypes($returnClassTypes);
					}

					$returnTypes = $method->getReturnTypes();
					if ($returnTypes !== null) {
						foreach ($returnTypes as $dataType => $returnType) {
							$symbolVariable->setDynamicTypes($dataType);
						}
					}

				}
			}
		}

		/**
		 * Some parameters in internal methods receive parameters as references
		 */
		if (isset($expression['parameters'])) {
			$references = array();
			if ($type == self::CALL_NORMAL || $type == self::CALL_DYNAMIC_STRING) {
				if (isset($method)) {
					if ($method instanceof ReflectionMethod) {
						$position = 0;
						foreach ($method->getParameters() as $parameter) {
							if ($parameter->isPassedByReference()) {
								$references[$position] = true;
							}
							$position++;
						}
					}
				}
			}
		}

		/**
		 * Include fcall header
		 */
		$compilationContext->headersManager->add('kernel/fcall');

		/**
		 * Call methods must grown the stack
		 */
		$compilationContext->symbolTable->mustGrownStack(true);

		/**
		 * Mark references
		 */
		if (isset($expression['parameters'])) {

			$params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
			if (count($references)) {
				foreach ($params as $position => $param) {
					if (isset($references[$position])) {
						$compilationContext->codePrinter->output('Z_SET_ISREF_P(' . $param . ');');
					}
				}
			}

			/**
			 * We check here if the correct type of parameter is passed to the called method
			 */
			if ($type == self::CALL_NORMAL) {
				if (isset($method) && $method instanceof ClassMethod && isset($expression['parameters'])) {
					$n = 0;
					$resolvedTypes = $this->getResolvedTypes();
					$resolvedDynamicTypes = $this->getResolvedDynamicTypes();
					foreach ($method->getParameters() as $parameter) {
						if (isset($parameter[0]['data-type'])) {

							/**
							 * If the passed parameter is different to the expected type we show a warning
							 */
							if ($resolvedTypes[$n] != $parameter[0]['data-type']) {

								/**
								 * Passing polymorphic variables to static typed parameters
								 * could lead to potential transformations
								 */
								if ($resolvedTypes[$n] == 'variable') {
									if ($resolvedDynamicTypes[$n] != $parameter[0]['data-type']) {
										if ($resolvedDynamicTypes[$n] == 'undefined') {
											$compilationContext->logger->warning("Passing possible incorrect type to parameter: " . $classDefinition->getCompleteName() . '::' . $parameter[0]['name'] . ', passing: ' . $resolvedDynamicTypes[$n] . ', ' .
												"expecting: " . $parameter[0]['data-type'], "possible-wrong-parameter-undefined", $expression);
										}
										//echo '1: ', $resolvedTypes[$n], ' ', $resolvedDynamicTypes[$n], ' ', $parameter[0]['data-type'], ' ', PHP_EOL;
									}
								} else {
									if ($parameter[0]['data-type'] != 'variable')	 {
										echo '2: ', $resolvedTypes[$n], ' ', $resolvedDynamicTypes[$n], ' ', $parameter[0]['data-type'], ' ', PHP_EOL;
									}
								}
							}
						}
						$n++;
					}
				}
			}

		}

		/**
		 * Generate the code according to the call type
		 */
		if ($type == self::CALL_NORMAL || $type == self::CALL_DYNAMIC_STRING) {

			if (!isset($expression['parameters']) || !count($params)) {
				if ($mustInit) {
					$symbolVariable->initVariant($compilationContext);
				}
				if ($compilationContext->insideCycle) {

					$functionCache = $compilationContext->symbolTable->getTempVariableForWrite('zend_function', $compilationContext);
					$functionCache->setMustInitNull(true);
					$functionCache->setReusable(false);

					if ($isExpecting) {
						$codePrinter->output('zephir_call_method_cache(' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', "' . $methodName . '", &' . $functionCache->getName() . ');');
					} else {
						$codePrinter->output('zephir_call_method_cache_noret(' . $variableVariable->getName() . ', "' . $methodName . '", &' . $functionCache->getName() . ');');
					}
				} else {
					if ($isExpecting) {
						$codePrinter->output('zephir_call_method(' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', "' . $methodName . '");');
					} else {
						$codePrinter->output('zephir_call_method_noret(' . $variableVariable->getName() . ', "' . $methodName . '");');
					}
				}
			} else {
				if ($mustInit) {
					$symbolVariable->initVariant($compilationContext);
				}
				if ($compilationContext->insideCycle) {

					$functionCache = $compilationContext->symbolTable->getTempVariableForWrite('zend_function', $compilationContext);
					$functionCache->setMustInitNull(true);
					$functionCache->setReusable(false);

					if ($isExpecting) {
						$codePrinter->output('zephir_call_method_p' . count($params) . '_cache(' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', "' . $methodName . '", &' . $functionCache->getName() . ', ' . join(', ', $params) . ');');
					} else {
						$codePrinter->output('zephir_call_method_p' . count($params) . '_cache_noret(' . $variableVariable->getName() . ', "' . $methodName . '", &' . $functionCache->getName() . ', ' . join(', ', $params) . ');');
					}
				} else {
					if ($isExpecting) {
						$codePrinter->output('zephir_call_method_p' . count($params) . '(' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', "' . $methodName . '", ' . join(', ', $params) . ');');
					} else {
						$codePrinter->output('zephir_call_method_p' . count($params) . '_noret(' . $variableVariable->getName() . ', "' . $methodName . '", ' . join(', ', $params) . ');');
					}
				}

			}

		} else {

			if (!isset($expression['parameters']) || !count($params)) {
				if ($mustInit) {
					$symbolVariable->initVariant($compilationContext);
				}
				if ($isExpecting) {
					$codePrinter->output('zephir_call_method_zval(' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $variableMethod->getName() . ');');
				} else {
					$codePrinter->output('zephir_call_method_zval_noret(' . $variableVariable->getName() . ', ' . $variableMethod->getName() . ');');
				}
			} else {
				if ($mustInit) {
					$symbolVariable->initVariant($compilationContext);
				}
				if ($isExpecting) {
					$codePrinter->output('zephir_call_method_zval_p' . count($params) . '(' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $variableMethod->getName() . ', ' . join(', ', $params) . ');');
				} else {
					$codePrinter->output('zephir_call_method_zval_p' . count($params) . '_noret(' . $variableVariable->getName() . ', ' . $variableMethod->getName() . ', ' . join(', ', $params) . ');');
				}
			}

		}

		/**
		 * We can mark temporary variables generated as idle
		 */
		foreach ($this->getTemporalVariables() as $tempVariable) {
			$tempVariable->setIdle(true);
		}

		/**
		 * Release parameters marked as references
		 */
		if (isset($expression['parameters'])) {
			if (count($references)) {
				foreach ($params as $position => $param) {
					if (isset($references[$position])) {
						$compilationContext->codePrinter->output('Z_UNSET_ISREF_P(' . $param . ');');
					}
				}
			}
		}

		if ($isExpecting) {
			return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
		}
		return new CompiledExpression('null', null, $expression);
	}

}
