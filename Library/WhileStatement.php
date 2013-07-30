<?php

/**
 * WhileStatement
 *
 * While statement, the same as in PHP/C
 */
class WhileStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	/**
	 * Perform the compilation of code
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$exprRaw = $this->_statement['expr'];

		$expr = new EvalExpression();
		$condition = $expr->optimize($exprRaw, $compilationContext);

		$compilationContext->codePrinter->output('while (' . $condition . ') {');

		/**
		 * Variables are initialized in a different way inside cycle
		 */
		$compilationContext->insideCycle++;

		/**
		 * Compile statements in the 'if' block
		 */
		if (isset($this->_statement['statements'])) {
			$st = new StatementsBlock($this->_statement['statements']);
			$st->compile($compilationContext);
		}

		/**
		 * Restore the cycle counter
		 */
		$compilationContext->insideCycle--;

		$compilationContext->codePrinter->output('}');

	}

}
