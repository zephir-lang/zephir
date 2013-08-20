<?php

/**
 * IsNumericOptimizer
 *
 * Optimizes calls to 'is_numeric' using internal function
 */
class IsNumericOptimizer
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

		$resolvedParams = $call->getResolvedParams($expression['parameters'], $context, $expression);
		return new CompiledExpression('bool', 'zephir_is_numeric(' . $resolvedParams[0] . ')', $expression);
	}
}