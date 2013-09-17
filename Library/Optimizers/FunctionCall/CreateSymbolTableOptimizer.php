<?php

/**
 * CreateSymbolTableOptimizer
 *
 * Built-in function that creates a virtual symbol table
 */
class CreateSymbolTableOptimizer
{
	/**
	 *
	 * @param array $expression
	 * @param Call $call
	 * @param CompilationContext $context
	 */
	public function optimize(array $expression, Call $call, CompilationContext $context)
	{

		if (isset($expression['parameters'])) {
			if (count($expression['parameters']) != 0) {
				throw new CompilerException("This function doesn't require parameters", $expression);
			}
		}

		/**
		 * Process the expected symbol to be returned
		 */
		$call->processExpectedReturn($context);

		$symbolVariable = $call->getSymbolVariable();
		if ($symbolVariable) {
			if ($symbolVariable->getType() != 'variable') {
				throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
			}
		}

		if ($call->mustInitSymbolVariable()) {
			$symbolVariable->initVariant($context);
		}

		$context->headersManager->add('kernel/memory');

		$context->codePrinter->output('zephir_create_symbol_table(TSRMLS_C);');
		$context->codePrinter->output('');

		return new CompiledExpression('null', null, $expression);
	}

}
