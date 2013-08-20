<?php

/**
 * IsCallableOptimizer
 *
 * Optimizes calls to 'is_callable' using internal function
 */
class IsCallableOptimizer
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

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
		return new CompiledExpression('bool', 'zephir_is_callable(' . $resolvedParams[0] . ' TSRMLS_CC)', $expression);
	}
}