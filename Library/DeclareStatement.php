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
	public function compile(CompilationContext $compilationContext)
	{
		$statement = $this->_statement;

		if (!isset($statement['data-type'])) {
			throw new Exception("Data type is required");
		}

		$pointer = null;
		switch ($statement['data-type']) {
			case 'int':
				$compilationContext->codePrinter->outputNoLineFeed('int ');
				break;
			case 'double':
				$compilationContext->codePrinter->outputNoLineFeed('double ');
				break;
			case 'variable':
				$pointer = '*';
				$compilationContext->codePrinter->outputNoLineFeed('zval ');
				break;
			default:
				throw new Exception("Unsupported type in declare " . $statement['data-type']);
		}

		$variables = array();
		foreach ($statement['variables'] as $variable) {

			if ($compilationContext->symbolTable->hasVariable($variable['variable'])) {
				throw new Exception("Variable '" . $variable['variable'] . "' is already defined");
			}

			$symbolVariable = $compilationContext->symbolTable->addVariable($statement['data-type'], $variable['variable']);

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

		$compilationContext->codePrinter->outputNoLevel(join(', ', $variables) . ';');
	}
}
