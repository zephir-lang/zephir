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
 * StaticCall
 *
 * Call methods in a static context
 */
class StaticCall extends Call
{

	/**
	 * Calls static methods on the 'self' context
	 *
	 * @param string $methodName
	 * @param array $expression
	 * @param Variable $symbolVariable
	 * @param boolean $mustInit
	 * @param boolean $isExpecting
	 * @param ClassDefinition $classDefinition
	 * @param CompilationContext $compilationContext
	 */
	protected function callSelf($methodName, array $expression, $symbolVariable, $mustInit, $isExpecting,
		ClassDefinition $classDefinition, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;
		$classCe = $classDefinition->getClassEntry();

		/**
		 * Call static methods must grown the stack
		 */
		$compilationContext->symbolTable->mustGrownStack(true);

		if (!isset($expression['parameters'])) {

			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}

			if ($isExpecting) {
				$codePrinter->output('zephir_call_self(' . $symbolVariable->getName() . ', this_ptr, "' . $methodName . '");');
			} else {
				//$codePrinter->output('zephir_call_self_noret(' . $symbolVariable->getName() . ', "' . $methodName . '");');
			}
		} else {

			$params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);

			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}

			if (count($params)) {
				if ($isExpecting) {
					$codePrinter->output('zephir_call_self_p' . count($params) . '(' . $symbolVariable->getName() . ', this_ptr, "' . $methodName . '", ' . join(', ', $params) . ');');
				} else {
					$codePrinter->output('zephir_call_self_p' . count($params) . '_noret(this_ptr, "' . $methodName . '", ' . join(', ', $params) . ');');
				}
			} else {
				if ($isExpecting) {
					$codePrinter->output('zephir_call_self(' . $symbolVariable->getName() . ', this_ptr, "' . $methodName . '");');
				} else {
					$codePrinter->output('zephir_call_self_noret("' . $className . '", "' . $methodName . '");');
				}
			}
		}
	}

	/**
	 * Calls static methods on the 'parent' context
	 *
	 * @param string $methodName
	 * @param array $expression
	 * @param Variable $symbolVariable
	 * @param boolean $mustInit
	 * @param boolean $isExpecting
	 * @param ClassDefinition $classDefinition
	 * @param CompilationContext $compilationContext
	 */
	protected function callParent($methodName, array $expression, $symbolVariable, $mustInit, $isExpecting,
		ClassDefinition $classDefinition, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;
		$classCe = $classDefinition->getClassEntry();

		/**
		 * Call static methods must grown the stack
		 */
		$compilationContext->symbolTable->mustGrownStack(true);

		if (!isset($expression['parameters'])) {

			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}

			if ($isExpecting) {
				$codePrinter->output('zephir_call_parent(' . $symbolVariable->getName() . ', this_ptr, ' . $classCe . ', "' . $methodName . '");');
			} else {
				$codePrinter->output('zephir_call_parent_noret(this_ptr, ' . $classCe . ', "' . $methodName . '");');
			}
		} else {

			$params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);

			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}

			if (count($params)) {
				if ($isExpecting) {
					$codePrinter->output('zephir_call_parent_p' . count($params) . '(' . $symbolVariable->getName() . ', this_ptr, ' . $classCe . ', "' . $methodName . '", ' . join(', ', $params) . ');');
				} else {
					$codePrinter->output('zephir_call_parent_p' . count($params) . '_noret(this_ptr, ' . $classCe . ', "' . $methodName . '", ' . join(', ', $params) . ');');
				}
			} else {
				if ($isExpecting) {
					$codePrinter->output('zephir_call_parent(' . $symbolVariable->getName() . ', this_ptr, "' . $className . '", "' . $methodName . '");');
				} else {
					$codePrinter->output('zephir_call_parent("' . $className . '", "' . $methodName . '");');
				}
			}
		}
	}

	/**
	 * Calls static methods on the some class context
	 *
	 * @param string $methodName
	 * @param array $expression
	 * @param Variable $symbolVariable
	 * @param boolean $mustInit
	 * @param boolean $isExpecting
	 * @param ClassDefinition $classDefinition
	 * @param CompilationContext $compilationContext
	 */
	protected function callFromClass($methodName, array $expression, $symbolVariable, $mustInit, $isExpecting,
		ClassDefinition $classDefinition, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;
		$className = str_replace('\\', '\\\\', $classDefinition->getCompleteName());

		/**
		 * Call static methods must grown the stack
		 */
		$compilationContext->symbolTable->mustGrownStack(true);

		if (!isset($expression['parameters'])) {

			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}

			if ($isExpecting) {
				$codePrinter->output('zephir_call_static(' . $symbolVariable->getName() . ', "' . $className . '", "' . $methodName . '");');
			} else {
				$codePrinter->output('zephir_call_static_noret("' . $className . '", "' . $methodName . '");');
			}
		} else {

			$params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);

			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}

			if (count($params)) {
				if ($isExpecting) {
					$codePrinter->output('zephir_call_static_p' . count($params) . '(' . $symbolVariable->getName() . ', "' . $className . '", "' . $methodName . '", ' . join(', ', $params) . ');');
				} else {
					$codePrinter->output('zephir_call_static_p' . count($params) . '_noret("' . $className . '", "' . $methodName . '", ' . join(', ', $params) . ');');
				}
			} else {
				if ($isExpecting) {
					$codePrinter->output('zephir_call_static(' . $symbolVariable->getName() . ', "' . $className . '", "' . $methodName . '");');
				} else {
					$codePrinter->output('zephir_call_static("' . $className . '", "' . $methodName . '");');
				}
			}
		}
	}

	/**
	 * Compiles a static method call
	 *
	 * @param array $expr
	 * @param CompilationContext $compilationContext
	 */
	public function compile(Expression $expr, CompilationContext $compilationContext)
	{

		$expression = $expr->getExpression();

		/**
		 * TODO: implement dynamic calls
		 */
		if (array_key_exists('dynamic-class', $expression) && $expression['dynamic-class']) {
			return new CompiledExpression('null', null, $expression);
		}

		$methodName = strtolower($expression['name']);

		$symbolVariable = null;
		$type = null;

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

			/**
			 * At this point, we don't know the exact dynamic type returned by the static method call
			 */
			$symbolVariable->setDynamicTypes('undefined');

			if ($symbolVariable->getType() != 'variable') {
				throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
			}
		}

		/**
		 * Include fcall header
		 */
		$compilationContext->headersManager->add('kernel/fcall');

		$compiler = $compilationContext->compiler;

		$className = $expression['class'];

		if ($className != 'self' && $className != 'parent') {
			if ($compiler->isClass($className)) {
				$classDefinition = $compiler->getClassDefinition($className);
			} else {
				throw new CompilerException("Incorrect class name: " . $expression['class'], $expression);
			}
		} else {
			if ($className == 'self') {
				$classDefinition = $compilationContext->classDefinition;
			} else {
				if ($className == 'parent') {
					$classDefinition = $compilationContext->classDefinition;
					$extendsClass = $classDefinition->getExtendsClass();
					if (!$extendsClass) {
						throw new CompilerException('Cannot call method "' . $methodName . '" on parent because class ' .
							$classDefinition->getCompleteName() . ' does not extend any class', $expression);
					} else {
						$currentClassDefinition = $classDefinition;
						$classDefinition = $classDefinition->getExtendsClassDefinition();
					}
				}
			}
		}

		/**
		 * Check if the class implements the method
		 */
		if (!$classDefinition->hasMethod($methodName)) {
			throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not implement static method: '" . $expression['name'] . "'", $expression);
		} else {
			if (!$classDefinition->hasMethod("__callStatic")) {
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
				$method = $classDefinition->getMethod("__callStatic");

				if ($method->isPrivate() && $method->getClassDefinition() != $compilationContext->classDefinition) {
					throw new CompilerException("Cannot call private magic method '__call' out of its scope", $expression);
				}
				if ($method->isProtected() && $method->getClassDefinition() != $compilationContext->classDefinition && $method->getClassDefinition() != $compilationContext->classDefinition->getExtendsClass()) {
					throw new CompilerException("Cannot call protected magic method '__call' out of its scope", $expression);
				}
			}
		}

		/**
		 * Call static methods in the same class, use the special context 'self'
		 */
		if ($className == 'self' || $classDefinition == $compilationContext->classDefinition) {
			$type = 'self';
		} else {
			if ($className == 'parent') {
				$type = 'parent';
			}
		}

		/**
		 * Call static methods in the 'self' context
		 */
		if ($type == 'self') {
			$this->callSelf($methodName, $expression, $symbolVariable, $mustInit,
				$isExpecting, $classDefinition, $compilationContext);
		} else {
			if ($type == 'parent') {
				$this->callParent($methodName, $expression, $symbolVariable, $mustInit,
					$isExpecting, $currentClassDefinition, $compilationContext);
			} else {
				$this->callFromClass($methodName, $expression, $symbolVariable, $mustInit,
					$isExpecting, $classDefinition, $compilationContext);
			}
		}

		/**
		 * Transfer the return type-hint to the returned variable
		 */
		if ($isExpecting) {
			if (isset($method)) {
				if ($method instanceof ClassMethod) {

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
		 * We can mark temporary variables generated as idle here
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
