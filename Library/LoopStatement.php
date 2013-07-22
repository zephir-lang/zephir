<?php

/**
 * LoopStatement
 *
 * Loop statement, infinite loop
 */
class LoopStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	/**
	 *
	 */
	public function compile(CompilationContext $compilationContext)
	{

		$compilationContext->codePrinter->output('while (1) {');

		/**
		 * Compile statements in the 'if' block
		 */
		if (isset($this->_statement['statements'])) {
			$st = new StatementsBlock($this->_statement['statements']);
			$st->compile($compilationContext);
		}

		$compilationContext->codePrinter->output('}');

	}

}
