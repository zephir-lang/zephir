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

				$variable = $symbolTable->getVariable($expr['value']);

				switch ($variable->getType()) {
					case VAR_TYPE_INT:
						$variable->increaseUses();
						$codePrinter->output('fprintf(stdout, "%d", ' . $expr['value'] . ');');
						break;
					default:
						throw new Exception("Unknown type: " . $variable->getType());
				}

				
		}
		
	}
}