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
	public function optimize(array $expression, Call $call, CompilationContext $context)
	{
		if (!isset($expression['parameters'])) {
			return false;
		}

		if (count($expression['parameters']) != 1) {
			return false;
		}

		$resolvedParams = $call->getResolvedParams($expression['parameters'], $context, $expression);
		return new CompiledExpression('int', 'zephir_fast_count_ev(' . $resolvedParams[0] . ' TSRMLS_CC)', $expression);
	}
}