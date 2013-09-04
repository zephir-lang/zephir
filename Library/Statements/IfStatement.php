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

/**
 * IfStatement
 *
 * If statement, the same as in PHP/C
 */
class IfStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	/**
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{

		$condition = false;

		/**
		 * This pass tries to move dynamic variable initialization out
		 * of the if/else branch
		 */
		if (isset($this->_statement['statements']) && isset($this->_statement['else_statements'])) {

			$skipVariantInit = new SkipVariantInit();

			$skipVariantInit->pass(0, new StatementsBlock($this->_statement['statements']));
			$skipVariantInit->pass(1, new StatementsBlock($this->_statement['else_statements']));

			$symbolTable = $compilationContext->symbolTable;
			foreach ($skipVariantInit->getVariables() as $variable) {
				if ($symbolTable->hasVariable($variable)) {
					$symbolVariable = $symbolTable->getVariable($variable);
					if ($symbolVariable->getType() == 'variable') {
						$symbolVariable->initVariant($compilationContext);
						$symbolVariable->skipInitVariant(2);
					}
				}
			}
		}

		$exprRaw = $this->_statement['expr'];

		$expr = new EvalExpression();
		$condition = $expr->optimize($exprRaw, $compilationContext);
		$compilationContext->codePrinter->output('if (' . $condition . ') {');

		/**
		 * Compile statements in the 'if' block
		 */
		if (isset($this->_statement['statements'])) {
			$st = new StatementsBlock($this->_statement['statements']);
			$st->compile($compilationContext);
		}

		/**
		 * Compile statements in the 'else' block
		 */
		if (isset($this->_statement['else_statements'])) {
			$compilationContext->codePrinter->output('} else {');
			$st = new StatementsBlock($this->_statement['else_statements']);
			$st->compile($compilationContext);
		}

		$compilationContext->codePrinter->output('}');

	}

}
