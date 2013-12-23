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
 * Empty
 *
 * Checks if a variable is empty string or null
 */
class EmptyOperator extends BaseOperator
{

	/**
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{
		$compilationContext->headersManager->add('kernel/operators');

        return $this->evaluateVariableExpression($expression, $compilationContext);
	}

    /**
	 * Evaluates variable expressions like
	 * if empty $a
	 * if !empty $b
	 *
	 * it will create something around below lines:
	 * ZEPHIR_IS_EMPTY(var)
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	protected function evaluateVariableExpression(
		$expression,
		CompilationContext $compilationContext
	) {

        switch (true) {
            case isset($expression['left']['left']['value'])
                && isset($expression['left']['left']['type'])
                && $expression['left']['left']['type'] == 'variable':

                $name = $expression['left']['left']['value'];

                break;

            default:
                throw new Exception(
                    'Empty syntax not supported'
                );
        }

		$variable = $compilationContext->symbolTable->getVariableForWrite(
			$name,
			$compilationContext,
			$expression['left']['left']
		);

		return new CompiledExpression(
			'bool',
			'ZEPHIR_IS_EMPTY(' . $variable->getName() . ')',
			$expression
		);
    }
}