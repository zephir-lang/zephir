<?php

/**
 * Is_callableOptimizer
 *
 * Optimizes calls to 'is_callable' using internal function
 */
class Is_callableOptimizer
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
		return new CompiledExpression('bool', 'zephir_is_callable(' . $resolvedParams[0] . ')', $expression);
	}
}