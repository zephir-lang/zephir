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

				$variable = $symbolTable->getVariableForRead($expr['value']);

				switch ($variable->getType()) {
					case VAR_TYPE_INT:						
						$codePrinter->output('fprintf(stdout, "%d", ' . $expr['value'] . ');');
						break;
					default:
						throw new Exception("Unknown type: " . $variable->getType());
				}				
		}		
	}
	
}