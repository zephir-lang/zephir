<?php

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
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$expr = new Expression($this->_statement['expr']);

		$compiledExpression = $expr->compile($compilationContext);

		/**
		 * Generate the condition according to the value returned by the evaluted expression
		 */
		switch ($compiledExpression->getType()) {
			case 'bool':
				$compilationContext->codePrinter->output('if (' . $compiledExpression->getCode() . ') {');
				break;
			default:
				throw new Exception("Expression can't be evaluated");

		}

		/**
		 * Compile statements in the if block
		 */
		if (isset($this->_statement['statements'])) {
			$st = new StatementsBlock($this->_statement['statements']);
			$st->compile($compilationContext);
		}

		//print_r($this->_statement);

		$compilationContext->codePrinter->output('}');
	}
}
