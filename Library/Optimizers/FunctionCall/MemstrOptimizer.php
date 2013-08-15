<?php

/**
 * MemnstrOptimizer
 *
 * Like 'strpos' but it returns a boolean value
 */
class MemstrOptimizer
{
	/**
	 *
	 */
	public function optimize(array $expression, array $resolvedParams, CompilationContext $context)
	{
		if (!isset($expression['parameters'])) {
			return false;
		}

		if (count($expression['parameters']) != 2) {
			return false;
		}

		return new CompiledExpression('bool', 'zephir_memnstr(' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ')', $expression);
	}
}