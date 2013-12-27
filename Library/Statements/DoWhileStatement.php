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
 * DoWhileStatement
 *
 * DoWhile statement, the same as in PHP/C
 */
class DoWhileStatement
	extends StatementAbstract
{
	/**
	 * @param CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$exprRaw = $this->_statement['expr'];

		$codePrinter = $compilationContext->codePrinter;

		$numberPrints = $codePrinter->getNumberPrints();

		$expr = new EvalExpression();
		$condition = $expr->optimize($exprRaw, $compilationContext);

		/**
		 * Compound conditions can be evaluated in a single line of the C-code
		 */
		if (($codePrinter->getNumberPrints() - $numberPrints) == 0) {
			$codePrinter->output('while (' . $condition . ') {');
		} else {
			$codePrinter->output('while (1) {');
			$codePrinter->outputLineBreak();
		}

		/**
		 * Variables are initialized in a different way inside cycle
		 */
		$compilationContext->insideCycle++;

		/**
		 * Compile statements in the 'while' block
		 */
		if (isset($this->_statement['statements'])) {
			$st = new StatementsBlock($this->_statement['statements']);
			$st->compile($compilationContext);
		}

		/**
		 * Restore the cycle counter
		 */
		$compilationContext->insideCycle--;

		$codePrinter->output('}');

	}

}
