<?php

/**
 * StartsWithOptimizer
 *
 * Like 'strpos' but it returns a boolean value
 */
class StartsWithOptimizer
{

	/**
	 *
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
			$str = $expression['parameters'][1]['value'];
			unset($expression['parameters'][1]);
		}

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

		$context->headersManager->add('kernel/string');

		if (isset($str)) {
			return new CompiledExpression('bool', 'zephir_start_with_str(' . $resolvedParams[0] . ', SL("' . $str . '"))', $expression);
		}

		return new CompiledExpression('bool', 'zephir_start_with(' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ')', $expression);
	}

}