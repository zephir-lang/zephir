<?php

/**
 * ContinueStatement
 *
 * Continue statement
 */
class ContinueStatement
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
		$compilationContext->codePrinter->output('continue;');
	}

}
