<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
 * CallUserFuncArrayOptimizer
 *
 * Optimizer for 'call_user_func_array'
 */
class CallUserFuncArrayOptimizer
	extends OptimizerAbstract
{
	/**
	 * @param array $expression
	 * @param Call $call
	 * @param CompilationContext $context
	 * @return bool|CompiledExpression|mixed
	 */
	public function optimize(array $expression, Call $call, CompilationContext $context)
	{
		if (!isset($expression['parameters'])) {
			return false;
		}

		if (count($expression['parameters']) != 2) {
			return false;
		}

		$symbolVariable = $call->getSymbolVariable();
		if ($symbolVariable) {
			if ($symbolVariable->getType() != 'variable') {
				throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
			}
			if ($call->mustInitSymbolVariable()) {
				$symbolVariable->initVariant($context);
			}
		} else {
			$symbolVariable = $context->symbolTable->addTemp('variable', $context);
			$symbolVariable->initVariant($context);
		}

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

		$context->headersManager->add('kernel/fcall');

		$context->codePrinter->output('ZEPHIR_CALL_USER_FUNC_ARRAY(' . $symbolVariable->getName() . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');');
		return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
	}

}
