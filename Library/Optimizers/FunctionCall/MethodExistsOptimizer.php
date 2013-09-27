<?php

/**
 * MethodExistsOptimizer
 *
 * Optimizes calls to 'method_exists' using internal function
 */
class MethodExistsOptimizer
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
			return false;
		}

		if (count($expression['parameters']) != 2) {
			return false;
		}

		if ($expression['parameters'][1]['type'] == 'string') {
			$str = Utils::addSlaches($expression['parameters'][1]['value']);
			unset($expression['parameters'][1]);
		}

		$context->headersManager->add('kernel/object');

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
		if (isset($str)) {
			return new CompiledExpression('int', 'zephir_method_exists_str(' . $resolvedParams[0] . ', SS("' . strtolower($str) . '") TSRMLS_CC)', $expression);
		}

		return new CompiledExpression('int', 'zephir_method_exists(' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ' TSRMLS_CC)', $expression);
	}

}
