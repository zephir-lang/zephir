<?php

class StatementsBlock
{
	protected $_statements;

	public function __construct(array $statements)
	{	
		$this->_statements = $statements;
	}	

	public function compile(CodePrinter $codePrinter, SymbolTable $symbolTable)
	{		
		foreach ($this->_statements as $statement) {
			switch ($statement['type']) {
				case 'echo':
					$echoStatement = new EchoStatement($statement);
					$echoStatement->compile($codePrinter, $symbolTable);
					break;
				case 'declare':
					$declareStatement = new DeclareStatement($statement);
					$declareStatement->compile($codePrinter, $symbolTable);
					break;
			}
		}		
	}
}