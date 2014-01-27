<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
 | Authors: Rack Lin <racklin@gmail.com>                                    |
 +--------------------------------------------------------------------------+
*/

/**
 * PregMatchOptimizer
 *
 * Optimizes calls to 'preg_match' using internal function
 */
class PregMatchOptimizer
	extends OptimizerAbstract
{

	protected static $GLOBAL_MATCH = 0;

	/**
	 * @param array $expression
	 * @param Call $call
	 * @param CompilationContext $context
	 * @return bool|CompiledExpression|mixed
	 */
	public function optimize(array $expression, Call $call, CompilationContext $context)
	{

		if (!isset($expression['parameters'])) {
			return false;
		}

		if (count($expression['parameters']) < 2) {
			return false;
		}

		/*
		 * NOT support for flags and offset now, fallback to PHP userland function.
		 */
		if (count($expression['parameters']) > 3) {
			return false;
		}

		/**
		 * Process the matches result
		 */
		if (count($expression['parameters']) == 3 && $expression['parameters'][2]['type'] == 'variable') {
			$matchesVariable = $context->symbolTable->getVariable($expression['parameters'][2]['value']);
			if (!$matchesVariable->isInitialized()) {
				$matchesVariable->initVariant($context);
				$matchesVariable->setIsInitialized(true);
			}
		} else {
			$matchesVariable = $context->symbolTable->addTemp('variable', $context);
			$matchesVariable->initVariant($context);
		}

		$matchesVariable->setDynamicTypes('array');

		/**
		 * Process the expected symbol to be returned
		 */
		$call->processExpectedReturn($context);

		$symbolVariable = $call->getSymbolVariable();
		if ($symbolVariable) {
			if ($symbolVariable->getType() != 'variable') {
					throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
			}
			if ($call->mustInitSymbolVariable()) {
				$symbolVariable->initVariant($context);
			}
		} else {
			$symbolVariable = $context->symbolTable->addTemp('variable', $context);
			$symbolVariable->initVariant($context);
		}

		$context->headersManager->add('kernel/string');

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
		$context->codePrinter->output('zephir_preg_match(' . $symbolVariable->getName() . ', &(' . $symbolVariable->getName() . '), ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $matchesVariable->getName() . ', ' . static::$GLOBAL_MATCH .' TSRMLS_CC);');
		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}
}
