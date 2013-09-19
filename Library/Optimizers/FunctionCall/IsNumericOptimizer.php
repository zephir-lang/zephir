<?php

/**
 * IsNumericOptimizer
 *
 * Optimizes calls to 'is_numeric' using internal function
 */
class IsNumericOptimizer
{
	/**
	 * @param array $expression
	 * @param Call $call
	 * @param CompilationContext $context
	 */
	public function optimize(array $expression, Call $call, CompilationContext $context)
	{
		if (!isset($expression['parameters'])) {
			return false;
		}

		if (count($expression['parameters']) != 1) {
			return false;
		}

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
		return new CompiledExpression('bool', 'zephir_is_numeric(' . $resolvedParams[0] . ')', $expression);
	}
}