<?php

class Expression
{

	protected $_expression;

	public function __construct(array $expression)
	{
		$this->_expression = $expression;
	}

	public function compile(SymbolTable $symbolTable)
	{

	}
	
}