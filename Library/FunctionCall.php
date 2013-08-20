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

	static protected $_optimizers = array();

	/**
	 * Tries to find specific an specialized optimizer for function calls
	 *
	 * @param string $funcName
	 * @param array $expression
	 */
	protected function optimize($funcName, array $expression, Call $call, CompilationContext $compilationContext)
	{
		/**
		 * Check if the optimizer is already cached
		 */
		if (!isset(self::$_optimizers[$funcName])) {

			$camelizeFunctionName = Utils::camelize($funcName);

			$path = 'Library/Optimizers/FunctionCall/' . $camelizeFunctionName . 'Optimizer.php';
			if (file_exists($path)) {

				require $path;

				$className = $camelizeFunctionName . 'Optimizer';
				$optimizer = new $className();
			} else {
				$optimizer = null;
			}

			self::$_optimizers[$funcName] = $optimizer;

		} else {
			$optimizer = self::$_optimizers[$funcName];
		}

		if ($optimizer) {
			return $optimizer->optimize($expression, $call, $compilationContext);
		}

		return false;
	}

	/**
	 * Compiles a function
	 *
	 * @param Expression $expr
	 * @param CompilationContext $expr
	 */
	public function compile(Expression $expr, CompilationContext $compilationContext)
	{

		$expression = $expr->getExpression();

		$funcName = strtolower($expression['name']);

		/**
		 * Try to optimize function calls
		 */
		$compiledExpr = $this->optimize($funcName, $expression, $this, $compilationContext);
		if (is_object($compiledExpr)) {
			return $compiledExpr;
		}

		/**
		 * Resolve parameters
		 */
		if (isset($expression['parameters'])) {
			$params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
		} else {
			$params = array();
		}

		$codePrinter = $compilationContext->codePrinter;

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

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
		}

		/**
		 * Include fcall header
		 */
		$compilationContext->headersManager->add('kernel/fcall');

		if (!isset($expression['parameters'])) {
			if ($isExpecting) {
				if ($mustInit) {
					$symbolVariable->initVariant($compilationContext);
				}
				$codePrinter->output('zephir_call_func(' . $symbolVariable->getName() . ', "' . $funcName . '");');
			} else {
				$codePrinter->output('zephir_call_func_noret("' . $funcName . '");');
			}
		} else {
			if (count($params)) {
				if ($isExpecting) {
					if ($mustInit) {
						$symbolVariable->initVariant($compilationContext);
					}
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