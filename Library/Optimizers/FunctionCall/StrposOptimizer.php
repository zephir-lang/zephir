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
 * StrposOptimizer
 *
 * Optimizes calls to 'strpos' using internal function
 */
class StrposOptimizer
	extends OptimizerAbstract
{
	/**
	 * @param array $expression
	 * @param Call $call
	 * @param CompilationContext $context
	 * @return bool|CompiledExpression|mixed
	 * @throws CompilerException
	 */
	public function optimize(array $expression, Call $call, CompilationContext $context)
	{
		if (!isset($expression['parameters'])) {
			return false;
		}

		if (count($expression['parameters']) < 2) {
			throw new CompilerException("'strpos' require two or three parameters");
		}

                /**
                 * Process offset
                 */
                $offset = '0 ';
                if (count($expression['parameters']) >= 3 && $expression['parameters'][2]['type'] == 'int') {
                    $offset = $expression['parameters'][2]['value'] . ' ';
                    unset($expression['parameters'][2]);
                }
		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
                if (count($resolvedParams) >= 3) {
                    $offset = 'Z_LVAL_P(' . $resolvedParams[2] . ') ';
                }

		/**
		 * Process the expected symbol to be returned
		 */
		$call->processExpectedReturn($context);

		$symbolVariable = $call->getSymbolVariable();
		if ($symbolVariable->isNotVariableAndString()) {
			throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
		}

		if ($call->mustInitSymbolVariable()) {
			$symbolVariable->initVariant($context);
		}

		$context->headersManager->add('kernel/string');

		$symbolVariable->setDynamicTypes('int');

		$context->codePrinter->output('zephir_fast_strpos(' . $symbolVariable->getName() . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $offset .');');
		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

}
