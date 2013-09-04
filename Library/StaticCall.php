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
 * Call methods in a static context
 */
class StaticCall extends Call
{

	protected function callSelf($methodName, array $expression, Variable $symbolVariable, $mustInit, $isExpecting,
		ClassDefinition $classDefinition, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;
		$classCe = $classDefinition->getClassEntry();

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
	 * Compiles a static method call
	 *
	 * @param array $expr
	 * @param CompilationContext $compilationContext
	 */
	public function compile(Expression $expr, CompilationContext $compilationContext)
	{

		$expression = $expr->getExpression();

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
			}
		}

		/**
		 * Call static methods in the same class, use the special context 'self'
		 */
		if ($className == 'self' || $classDefinition == $compilationContext->classDefinition) {
			$type = 'self';
		}

		/**
		 * Call static methods in the 'self' context
		 */
		if ($type == 'self') {
			$this->callSelf($methodName, $expression, $symbolVariable, $mustInit,
				$isExpecting, $classDefinition, $compilationContext);
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
