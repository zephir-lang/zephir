<?php

/**
 * SymbolTable
 *
 * A symbol table stores all the variables defined in a method, their data types
 * default values
 */
class SymbolTable
{
	protected $_variables = array();
	
	public function hasVariable($name)
	{
		return isset($this->_variables[$name]);
	}

	/**
	 * Adds a variable to the symbol table
	 *
	 * @param int $type
	 * @param string $name
	 * @return \Variable
	 */
	public function addVariable($type, $name)
	{
		$variable = new Variable($type, $name);
		$this->_variables[$name] = $variable;
		return $variable;
	}

	/**
	 * Returns a variable in the symbol table
	 *
	 * @return \Variable
	 */ 
	public function getVariable($name)
	{
		return $this->_variables[$name];
	}

	/**
	 * Returns all the variables defined in the symbol table
	 *
	 * @return \Variable[]
	 */
	public function getVariables()
	{
		return $this->_variables;
	}

}