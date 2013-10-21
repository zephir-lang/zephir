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
 * Clone
 *
 * Clones an object into another one
 */
class CloneOperator extends BaseOperator
{

	/**
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{

		$compilationContext->headersManager->add('kernel/object');

		$exprVariable = new Expression($expression['left']);
		$exprVariable->setReadOnly(true);
		$exprVariable->setExpectReturn(true);
		$exprCompiledVariable = $exprVariable->compile($compilationContext);
		if ($exprCompiledVariable->getType() != 'variable') {
			throw new CompiledException("Expression type: " . $exprCompiledVariable->getType() . " cannot be used as array", $expression);
		}

		$clonedVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression);
		if ($clonedVariable->getType() != 'variable') {
			throw new CompiledException("Variable type: " . $variable->getType() . " cannot be cloned");
		}

		if ($clonedVariable->hasDifferentDynamicType(array('undefined', 'object', 'null'))) {
			$compilationContext->logger->warning('Possible attempt to use non array in fetch operator', 'non-valid-clone', $expression);
		}

		$symbolVariable = $this->getExpected($compilationContext, $expression);

		$symbolVariable->setDynamicTypes('object');

		$symbolVariable->setIsInitialized(true);

		/* Inherit the dynamic type data from the cloned object */
		$symbolVariable->setDynamicTypes($clonedVariable->getDynamicTypes());
		$symbolVariable->setClassTypes($clonedVariable->getClassTypes());

		$compilationContext->codePrinter->output('if (zephir_clone(' . $symbolVariable->getName() . ', ' . $clonedVariable->getName() . ' TSRMLS_CC) == FAILURE) {');
		$compilationContext->codePrinter->output("\t" . 'RETURN_MM();');
        $compilationContext->codePrinter->output('}');

		return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
	}

}