<?php

/**
 * BreakStatement
 *
 * Break statement
 */
class BreakStatement
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
		if ($compilationContext->insideCycle) {
			$compilationContext->codePrinter->output('break;');
		} else {
			throw new CompilerException("Cannot use 'break' outside a cycle", $this->_statement);
		}
	}

}
