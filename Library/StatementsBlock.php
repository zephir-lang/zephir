<?php

/**
 * StatementsBlock
 *
 * This represent a list of statements like the one in a method
 * or a if/else/while
 */
class StatementsBlock
{
	protected $_statements;

	public function __construct(array $statements)
	{
		$this->_statements = $statements;
	}

	public function compile(CodePrinter $codePrinter, SymbolTable $symbolTable, ClassDefinition $classDefinition=null)
	{
		foreach ($this->_statements as $statement) {
			switch ($statement['type']) {
				case 'let':
					$letStatement = new LetStatement($statement);
					$letStatement->compile($codePrinter, $symbolTable, $classDefinition);
					break;
				case 'echo':
					$echoStatement = new EchoStatement($statement);
					$echoStatement->compile($codePrinter, $symbolTable, $classDefinition);
					break;
				case 'declare':
					$declareStatement = new DeclareStatement($statement);
					$declareStatement->compile($codePrinter, $symbolTable, $classDefinition);
					break;
				case 'if':
					$ifStatement = new IfStatement($statement);
					$ifStatement->compile($codePrinter, $symbolTable, $classDefinition);
					break;
			}
		}
	}
}