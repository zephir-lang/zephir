<?php

/**
 * SymbolTable
 *
 * A symbol table stores all the variables defined in a method, their data types
 * default values
 */
class SymbolTable
{
	protected $_variables;
	
	public function hasVariable($name)
	{
		return isset($this->_variables[$name]);
	}

	public function addVariable($type, $name)
	{
		$variable = new Variable($type, $name);
		$this->_variables[$name] = $variable;
		return $variable;
	}

	public function getVariable()
	{
		
	}

}