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
 * CreateInstanceOptimizer
 *
 * Built-in function that creates new instances of objects from its class name
 */
class CreateInstanceOptimizer
{
	/**
	 *
	 * @param array $expression
	 * @param Call $call
	 * @param CompilationContext $context
	 */
	public function optimize(array $expression, Call $call, CompilationContext $context)
	{
		if (!isset($expression['parameters'])) {
			throw new CompilerException("This function requires parameters", $expression);
		}

		if (count($expression['parameters']) != 1) {
			throw new CompilerException("This function only requires one parameter", $expression);
		}

		/**
		 * Process the expected symbol to be returned
		 */
		$call->processExpectedReturn($context);

		$symbolVariable = $call->getSymbolVariable();
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
		}

		if ($call->mustInitSymbolVariable()) {
			$symbolVariable->initVariant($context);
		}

		$context->headersManager->add('kernel/object');

		$symbolVariable->setDynamicTypes('object');

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

		$context->codePrinter->output('if (zephir_create_instance(' . $symbolVariable->getName() . ', ' . $resolvedParams[0] . ' TSRMLS_CC) == FAILURE) {');
		$context->codePrinter->output("\t" . 'return;');
		$context->codePrinter->output('}');

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

}
