<?php

/**
 * CountOptimizer
 *
 * Optimizes calls to 'count' using internal function
 */
class CountOptimizer
{
	/**
	 *
	 */
	public function optimize(array $expression, array $resolvedParams, CompilationContext $context)
	{
		if (!isset($expression['parameters'])) {
			return false;
		}

		if (count($expression['parameters']) != 1) {
			return false;
		}

		return new CompiledExpression('int', 'zephir_fast_count_ev(' . $resolvedParams[0] . ')', $expression);
	}
}