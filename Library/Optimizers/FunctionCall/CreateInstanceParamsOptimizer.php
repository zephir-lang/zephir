<?php

/**
 * CreateInstanceParamsOptimizer
 *
 * Built-in function that creates new instances of objects from its class name passing parameters as an array
 */
class CreateInstanceParamsOptimizer
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

		if (count($expression['parameters']) != 2) {
			throw new CompilerException("This function only requires two parameter", $expression);
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

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

		$context->codePrinter->output('if (zephir_create_instance_params(' . $symbolVariable->getName() . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ' TSRMLS_CC) == FAILURE) {');
		$context->codePrinter->output("\t" . 'return;');
		$context->codePrinter->output('}');

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

}
