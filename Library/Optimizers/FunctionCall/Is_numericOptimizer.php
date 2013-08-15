<?php

/**
 * Is_numericOptimizer
 *
 * Optimizes calls to 'count' using internal function
 */
class Is_numericOptimizer
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

		return new CompiledExpression('bool', 'zephir_is_numeric(' . $resolvedParams[0] . ')', $expression);
	}
}