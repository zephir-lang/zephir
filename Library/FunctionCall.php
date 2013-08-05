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
 * FunctionCall
 *
 * Call functions. By default functions are called in the PHP userland if an optimizer
 * is not found or there is not a user handler for it
 */
class FunctionCall extends Call
{

	/**
	 * Compiles a function
	 *
	 * @param Expression $expr
	 * @param CompilationContext $expr
	 */
	public function compile(Expression $expr, CompilationContext $compilationContext)
	{

		$expression = $expr->getExpression();

		$codePrinter = $compilationContext->codePrinter;

		$funcName = strtolower($expression['name']);

		/**
		 * Create temporary variable if needed
		 */
		$isExpecting = $expr->isExpectingReturn();
		if ($isExpecting) {
			$symbolVariable = $expr->getExpectingVariable();
			if (is_object($symbolVariable)) {
				$symbolVariable->initVariant($compilationContext);
			} else {
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
			}
		}

		/**
		 * Include fcall header
		 */
		$compilationContext->headersManager->add('kernel/fcall');

		if (!isset($expression['parameters'])) {
			if ($isExpecting) {
				$codePrinter->output('zephir_call_func(' . $symbolVariable->getName() . ', "' . $funcName . '");');
			} else {
				$codePrinter->output('zephir_call_func_noret("' . $funcName . '");');
			}
		} else {

			/**
			 * @TODO: Resolve parameters properly
			 */
			$params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
			if (count($params)) {
				if ($isExpecting) {
					$codePrinter->output('zephir_call_func_p' . count($params) . '(' . $symbolVariable->getName() . ', "' . $funcName . '", ' . join(', ', $params) . ');');
				} else {
					$codePrinter->output('zephir_call_func_p' . count($params) . '_noret("' . $funcName . '", ' . join(', ', $params) . ');');
				}
			} else {
				$codePrinter->output('zephir_call_func_noret("' . $funcName . '");');
			}
		}

		if ($isExpecting) {
			return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
		}

		return new CompiledExpression('null', null, $expression);
	}

}