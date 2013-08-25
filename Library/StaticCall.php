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

class StaticCall extends Call
{

	public function compile(Expression $expr, CompilationContext $compilationContext)
	{

		$expression = $expr->getExpression();
		//return new CompiledExpression('null', null, $expression);

		$codePrinter = $compilationContext->codePrinter;

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

		//PHALCON_CALL_STATIC_PARAMS_2(html, "phalcon\\tag\\select", "selectfield", parameters, data);

		$className = strtolower(str_replace('\\', '\\\\', $expression['class']));

		/**
		 *
		 */
		if (!isset($expression['parameters'])) {
			if ($mustInit) {
				$symbolVariable->initVariant($compilationContext);
			}
			if ($isExpecting) {
				//$codePrinter->output('ZEPHIR_CALL_SELF(' . $symbolVariable->getName() . ', "' . $className . '", "' . $methodName . '");');
				$codePrinter->output('ZEPHIR_CALL_SELF(' . $symbolVariable->getName() . ', this_ptr, "' . $methodName . '");');
			} else {
				$codePrinter->output('ZEPHIR_CALL_STATIC_NORETURN("' . $className . '", "' . $methodName . '");');
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

		if ($isExpecting) {
			return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
		}
		return new CompiledExpression('null', null, $expression);
	}

}
