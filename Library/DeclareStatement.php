<?php

/**
 * DeclareStatement
 *
 * This creates variables in the current symbol table
 */
class DeclareStatement
{

	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	/**
	 * Compiles the statement
	 */
	public function compile(CodePrinter $codePrinter, SymbolTable $symbolTable)
	{
		$statement = $this->_statement;

		if (!isset($statement['data-type'])) {
			throw new Exception("Data type is required");
		}

		$pointer = null;
		switch ($statement['data-type']) {
			case VAR_TYPE_INT:
				$codePrinter->outputNoLineFeed('int ');
				break;
			case VAR_TYPE_DOUBLE:
				$codePrinter->outputNoLineFeed('double ');
				break;
			case VAR_TYPE_VAR:
				$pointer = '*';
				$codePrinter->outputNoLineFeed('zval ');
				break;
			default:
				throw new Exception("Unsupported type in declare");
		}

		$variables = array();
		foreach ($statement['variables'] as $variable) {

			if ($symbolTable->hasVariable($variable['variable'])) {
				throw new Exception("Variable '" . $variable['variable'] . "' is already defined");
			}

			$symbolVariable = $symbolTable->addVariable($statement['data-type'], $variable['variable']);

			if (isset($variable['expr'])) {
				$defaultValue = $variable['expr']['value'];
			} else {
				$defaultValue = null;
			}

			/**
			 * @TODO: default values must match the data type
			 */
			if ($defaultValue !== null) {
				$variables[] = $pointer . $variable['variable']. ' = ' . $defaultValue;
				$symbolVariable->setInitialized(true);
			} else {
				$variables[] = $pointer . $variable['variable'];
			}
		}

		$codePrinter->outputNoLevel(join(', ', $variables) . ';');
	}
}
