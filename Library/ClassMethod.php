<?php

/**
 * ClassMethod
 *
 * Represents a class method
 */
class ClassMethod
{

	protected $_visibility;

	protected $_name;

	protected $_statements;

	/**
	 * ClassMethod constructor
	 *
	 * @param string $visibility
	 */
	public function __construct($visibility, $name, StatementsBlock $statements=null)
	{
		$this->_visibility = $visibility;
		$this->_name = $name;
		$this->_statements = $statements;
	}

	public function getName()
	{
		return $this->_name;
	}

	public function getVisibilityAccesor()
	{
		return 'ZEND_ACC_PROTECTED';
	}

	/**
	 * Compiles the method
	 *
	 * @param CodePrinter $codePrinter
	 * @param ClassDefinition $classDefinition
	 */
	public function compile(CodePrinter $codePrinter, ClassDefinition $classDefinition)
	{

		$symbolTable = new SymbolTable();		

		if (is_object($this->_statements)) {
			$this->_statements->compile($codePrinter, $symbolTable);						
		}

		/**
		 * Check if there are unused variables
		 */
		foreach ($symbolTable->getVariables() as $variable) {
			if ($variable->getNumberUses() <= 0) {
				echo 'Warning: Variable "' . $variable->getName() . '" declreated but not used in ' . 
					$classDefinition->getName() . '::' . $this->getName(), PHP_EOL;
			}
		}				

		return null;
	}

}