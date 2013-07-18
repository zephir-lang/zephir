<?php

class Expression
{

	protected $_expression;

	public function __construct(array $expression)
	{
		$this->_expression = $expression;
	}

	/**
	 * Resolves an expression
	 *
	 */
	public function resolve(SymbolTable $symbolTable, ClassDefinition $classDefinition=null)
	{
		$type = $this->_expression['type'];
		switch ($type) {
			case 301:
				return $this->_expression;
				break;
			case 303:
				return $this->_expression;
				break;
		}
		echo $type;
	}

}