<?php

/**
 * DeclareStatement
 *
 *
 */
class DeclareStatement
{

	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	public function compile(CodePrinter $codePrinter, SymbolTable $symbolTable)
	{
		$statement = $this->_statement;

		if (!isset($statement['data-type'])) {
			throw new Exception("Data type is required");			
		}

		switch ($statement['data-type']) {
			case VAR_TYPE_INT:
				$codePrinter->outputNoLineFeed('int ');
				break;		
		}

		$variables = array();
		foreach ($statement['variables'] as $variable) {

			if ($symbolTable->hasVariable($variable['variable'])) {
				throw new Exception("Variable '" . $variable['variable'] . "' is already defined");				
			}

			$symbolTable->addVariable($statement['data-type'], $variable['variable']);

			$variables[] = $variable['variable'];
		}

		$codePrinter->outputNoLevel(join(', ', $variables) . ';');

	}
}
