<?php

class EchoStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	public function compile(CodePrinter $codePrinter, SymbolTable $symbolTable)
	{
		$expr = $this->_statement['expr'];
		switch ($expr['type']) {
			case EXPR_IDENTIFIER:
				if (!$symbolTable->hasVariable($expr['value'])) {
					throw new Exception("Cannot echo variable '" . $expr['value'] . "' because it wasn't defined");					
				}
				$codePrinter->output('fprintf(stdout, "%d\n", ' . $expr['value'] . ');');
		}
		
	}
}