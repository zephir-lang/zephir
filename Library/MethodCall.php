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

class MethodCall
{

	public function compile(Expression $expr, CompilationContext $compilationContext)
	{

		$expression = $expr->getExpression();

		$variableVariable = $compilationContext->symbolTable->getVariableForRead($expression['variable'], $expression);
		if ($variableVariable->getType() != 'variable') {
			throw new CompilerException("Methods cannot be called on variable type: " . $symbolVariable->getType(), $expression);
		}

		$codePrinter = $compilationContext->codePrinter;

		$methodName = strtolower($expression['name']);

		$isExpecting = $expr->isExpectingReturn();
		if ($isExpecting) {
			$symbolVariable = $expr->getExpectingVariable();
			if (is_object($symbolVariable)) {
				$symbolVariable->initVariant($compilationContext);
			} else {
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
			}
		}

		/**
		 *
		 */
		if (!isset($expression['parameters'])) {
			if ($isExpecting) {
				$codePrinter->output('zephir_call_method(' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', "' . $methodName . '");');
			} else {
				$codePrinter->output('zephir_call_method_noret(' . $variableVariable->getName() . ', "' . $methodName . '");');
			}
		} else {

			/**
			 * @TODO: Resolve parameters properly
			 */
			$params = array();
			if (isset($expression['parameters'])) {
				foreach ($expression['parameters'] as $parameter) {
					$paramExpr = new Expression($parameter);
					$compiledExpression = $paramExpr->compile($compilationContext);
					switch ($compiledExpression->getType()) {
						case 'string':
							$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
							$codePrinter->output('ZVAL_STRING(' . $parameterVariable->getName() . ', "' . $compiledExpression->getCode() . '", 1);');
							$params[] = $parameterVariable->getName();
							break;
						case 'variable':
							$parameterVariable = $compilationContext->symbolTable->getVariableForRead($expression['variable'], $expression);
							switch ($parameterVariable->getType()) {
								case 'variable':
									$params[] = $parameterVariable->getName();
									break;
								default:
									throw new CompilerException("Cannot use variable type: " . $compiledExpression->getType() . " as parameter", $parameter);
							}
							break;
						default:
							throw new CompilerException("Cannot use value type: " . $compiledExpression->getType() . " as parameter", $parameter);
					}
				}
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