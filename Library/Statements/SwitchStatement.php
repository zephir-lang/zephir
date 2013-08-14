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
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

/**
 * SwitchStatement
 *
 * Switch statement, the same as in PHP/C
 */
class SwitchStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	/**
	 * Perform the compilation of code
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$exprRaw = $this->_statement['expr'];

		$codePrinter = $compilationContext->codePrinter;

		$numberPrints = $codePrinter->getNumberPrints();

		$compilationContext->insideSwitch++;

		$codePrinter->output('do {');

		$compilationContext->codePrinter->increaseLevel();

		$evalExpr = new EvalExpression();
		foreach ($this->_statement['clauses'] as $clause) {
			if ($clause['type'] == 'case') {

				$expr = array(
					'type' => 'equals',
					'left' => $exprRaw,
					'right' => $clause['expr']
				);

				$condition = $evalExpr->optimize($expr, $compilationContext);
				$codePrinter->output('if (' . $condition . ') {');

				if (isset($clause['statements'])) {
					$st = new StatementsBlock($clause['statements']);
					$st->compile($compilationContext);
				}

				$codePrinter->output('}');
			}
		}

		$compilationContext->insideSwitch--;

		$compilationContext->codePrinter->decreaseLevel();

		$codePrinter->output('} while(0); ');

	}

}
