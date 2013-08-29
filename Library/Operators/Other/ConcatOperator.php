<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

class ConcatOperator extends BaseOperator
{

	public function compile($expression, CompilationContext $compilationContext)
	{

		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		$compilationContext->headersManager->add('kernel/concat');

		$leftExpr = new Expression($expression['left']);
		$leftExpr->setReadOnly($this->_readOnly);
		$left = $leftExpr->compile($compilationContext);

		$rightExpr = new Expression($expression['right']);
		$rightExpr->setReadOnly($this->_readOnly);
		$right = $rightExpr->compile($compilationContext);

		$expected = $this->getExpectedComplexLiteral($compilationContext, $expression);

		if ($left->getType() == 'string' && $right->getType() == 'variable') {
			$compilationContext->codePrinter->output('ZEPHIR_CONCAT_SV(' . $expected->getName() . ', "' . $left->getCode() . '", ' . $right->getCode() . ');');
		}

		if ($left->getType() == 'variable' && $right->getType() == 'string') {
			$compilationContext->codePrinter->output('ZEPHIR_CONCAT_VS(' . $expected->getName() . ', ' . $left->getCode() . ', "' . $right->getCode() . '");');
		}

		if ($left->getType() == 'variable' && $right->getType() == 'variable') {
			$compilationContext->codePrinter->output('concat_function(' . $expected->getName() . ', ' . $left->getCode() . ', ' . $right->getCode() . ');');
		}

		return new CompiledExpression('variable', $expected->getName(), $expression);
	}

}
