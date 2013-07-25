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
			throw new CompilerException("Data type is required", $this->_statement);
		}

		$variables = array();
		foreach ($statement['variables'] as $variable) {

			if ($compilationContext->symbolTable->hasVariable($variable['variable'])) {
				throw new CompilerException("Variable '" . $variable['variable'] . "' is already defined", $variable);
			}

			/**
			 * Variables are added to the symbol table
			 */
			if (isset($variable['expr'])) {
				$symbolVariable = $compilationContext->symbolTable->addVariable($statement['data-type'], $variable['variable'], $variable['expr']);
			} else {
				$symbolVariable = $compilationContext->symbolTable->addVariable($statement['data-type'], $variable['variable']);
			}

			/**
			 * Variables with a default value are initialized by default
			 */
			if (isset($variable['expr'])) {
				$symbolVariable->setInitialized(true);
			}

			/*if (isset($variable['expr'])) {
				$defaultValue = $variable['expr']['value'];
			} else {
				$defaultValue = null;
			}

			if ($defaultValue !== null) {
				$variables[] = $pointer . $variable['variable']. ' = ' . $defaultValue;
				$symbolVariable->setInitialized(true);
			} else {
				$variables[] = $pointer . $variable['variable'];
			}*/
		}

		//
	}
}
