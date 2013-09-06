<?php

/**
 * CamelizeOptimizer
 *
 * Optimizes calls to 'camelize' using internal function
 */
class CamelizeOptimizer
{
	/**
	 *
	 */
	public function optimize(array $expression, Call $call, CompilationContext $context)
	{
		if (!isset($expression['parameters'])) {
			return false;
		}

		if (count($expression['parameters']) != 1) {
			return false;
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

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
		$context->codePrinter->output('zephir_camelize(' . $symbolVariable->getName() . ', ' . $resolvedParams[0] . ');');
		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}
}