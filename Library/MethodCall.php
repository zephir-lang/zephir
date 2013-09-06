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

		$variableVariable = $compilationContext->symbolTable->getVariableForRead($expression['variable'], $compilationContext, $expression);
		if ($variableVariable->getType() != 'variable') {
			throw new CompilerException("Methods cannot be called on variable type: " . $symbolVariable->getType(), $expression);
		}

		$codePrinter = $compilationContext->codePrinter;

		$type = $expression['call-type'];
		$methodName = strtolower($expression['name']);

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
			$symbolVariable->setDynamicType('undefined');
		}

		/**
		 * Try to check if the method exist in the callee
		 */
		if ($type == self::CALL_NORMAL) {
			if ($variableVariable->getRealName() == 'this') {

				$classDefinition = $compilationContext->classDefinition;
				if (!$classDefinition->hasMethod($methodName)) {
					throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not implement method: '" . $expression['name'] . "'", $expression);
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
				if ($variableVariable->getDynamicType() == 'object') {

					$classType = $variableVariable->getClassType();
					$compiler = $compilationContext->compiler;
					if ($compiler->isClass($classType)) {

						$classDefinition = $compiler->getClassDefinition($classType);
						if (!$classDefinition) {
							throw new CompilerException("Cannot locate class definition for class " . $classType, $expression);
						}

						if (!$classDefinition->hasMethod($methodName)) {
							throw new CompilerException("Class '" . $classType . "' does not implement method: '" . $expression['name'] . "'", $expression);
						}

						/**
						 * Try to produce an exception if method is called with a wrong number of parameters
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
						if ($compiler->isInternalClass($classType)) {
							$classDefinition = $compiler->getInternalClassDefinition($classType);
							if (!$classDefinition->hasMethod($methodName)) {
								throw new CompilerException("Class '" . $classType . "' does not implement method: '" . $expression['name'] . "'", $expression);
							}
						} else {
							$compilationContext->logger->warning("Class \"" . $classType . "\" does not exist at compile time", "nonexistant-class", $expression);
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
		 * Generate the code according to parentheses
		 */
		if (!isset($expression['parameters'])) {
			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}
			if ($isExpecting) {
				$codePrinter->output('zephir_call_method(' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', "' . $methodName . '");');
			} else {
				$codePrinter->output('zephir_call_method_noret(' . $variableVariable->getName() . ', "' . $methodName . '");');
			}
		} else {

			$params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);

			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}

			if (count($params)) {
				if ($isExpecting) {
					$codePrinter->output('zephir_call_method_p' . count($params) . '(' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', "' . $methodName . '", ' . join(', ', $params) . ');');
				} else {
					$codePrinter->output('zephir_call_method_p' . count($params) . '_noret(' . $variableVariable->getName() . ', "' . $methodName . '", ' . join(', ', $params) . ');');
				}
			} else {
				$codePrinter->output('zephir_call_method_noret(' . $variableVariable->getName() . ', "' . $methodName . '");');
			}
		}

		/**
		 * We can mark temporary variables generated as idle
		 */
		foreach ($this->getTemporalVariables() as $tempVariable) {
			$tempVariable->setIdle(true);
		}

		if ($isExpecting) {
			return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
		}
		return new CompiledExpression('null', null, $expression);
	}

}
