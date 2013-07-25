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
		if ($compilationContext->insideCycle) {
			$compilationContext->codePrinter->output('continue;');
		} else {
			throw new CompilerException("Cannot use 'continue' outside a cycle", $this->_statement);
		}
	}

}
