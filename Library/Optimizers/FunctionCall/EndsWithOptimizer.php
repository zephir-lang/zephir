<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

/**
 * EndsWithOptimizer
 *
 * Like 'strpos' but it returns a boolean value
 */
class EndsWithOptimizer
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
			$str = $expression['parameters'][1]['value'];
			unset($expression['parameters'][1]);
		}

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

		$context->headersManager->add('kernel/string');

		if (isset($str)) {
			return new CompiledExpression('bool', 'zephir_end_with_str(' . $resolvedParams[0] . ', SL("' . $str . '"))', $expression);
		}

		return new CompiledExpression('bool', 'zephir_end_with(' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ')', $expression);
	}

}
