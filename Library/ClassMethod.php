<?php

class ClassMethod
{

	protected $_visibility;

	protected $_name;

	protected $_statements;

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

	public function compile(CodePrinter $codePrinter, ClassDefinition $classDefinition)
	{

		$symbolTable = new SymbolTable();		

		if (is_object($this->_statements)) {
			$this->_statements->compile($codePrinter, $symbolTable);						
		}				

		return null;
	}

}