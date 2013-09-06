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
				//$codePrinter->output('ZEPHIR_CALL_SELF(' . $symbolVariable->getName() . ', "' . $className . '", "' . $methodName . '");');
				$codePrinter->output('ZEPHIR_CALL_SELF(' . $symbolVariable->getName() . ', this_ptr, ' . $classCe . ', "' . $methodName . '");');
			} else {
				$codePrinter->output('ZEPHIR_CALL_SELF_NORETURN("' . $className . '", "' . $methodName . '");');
			}
		} else {

			$params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);

			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}

			if (count($params)) {
				if ($isExpecting) {
					$codePrinter->output('ZEPHIR_CALL_SELF_PARAMS_' . count($params) . '(' . $symbolVariable->getName() . ', this_ptr, "' . $methodName . '", ' . join(', ', $params) . ');');
				} else {
					$codePrinter->output('ZEPHIR_CALL_SELF_PARAMS_' . count($params) . '_NORETURN(' . $variableVariable->getName() . ', this_ptr, "' . $methodName . '", ' . join(', ', $params) . ');');
				}
			} else {
				$codePrinter->output('zephir_call_method_noret(' . $variableVariable->getName() . ', "' . $methodName . '");');
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
				//$codePrinter->output('ZEPHIR_CALL_SELF(' . $symbolVariable->getName() . ', "' . $className . '", "' . $methodName . '");');
				$codePrinter->output('ZEPHIR_CALL_PARENT(' . $symbolVariable->getName() . ', this_ptr, "' . $className . '", "' . $methodName . '");');
			} else {
				$codePrinter->output('ZEPHIR_CALL_PARENT_NORETURN("' . $className . '", "' . $methodName . '");');
			}
		} else {

			$params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);

			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}

			if (count($params)) {
				if ($isExpecting) {
					$codePrinter->output('ZEPHIR_CALL_PARENT_PARAMS_' . count($params) . '(' . $symbolVariable->getName() . ', this_ptr, "' . $methodName . '", ' . join(', ', $params) . ');');
				} else {
					$codePrinter->output('ZEPHIR_CALL_PARENT_PARAMS_' . count($params) . '_NORETURN(this_ptr, "' . $methodName . '", ' . join(', ', $params) . ');');
				}
			} else {
				$codePrinter->output('zephir_call_method_noret(' . $variableVariable->getName() . ', "' . $methodName . '");');
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
			$symbolVariable->setDynamicType('undefined');

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
			throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not implement method: '" . $expression['name'] . "'", $expression);
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
