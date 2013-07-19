<?php

/**
 * SymbolTable
 *
 * A symbol table stores all the variables defined in a method, their data types
 * default values
 */
class SymbolTable
{
	protected $_mustGrownStack = false;

	protected $_variables = array();

	/**
	 * Check if a variable is declared in the current symbol table
	 *
	 * @param string $name
	 * @return boolean
	 */
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

	/**
	 * Return a variable in the symbol table, it will be used for a read operation
	 *
	 * @return \Variable
	 */
	public function getVariableForRead($name)
	{

		if (!$this->hasVariable($name)) {
			throw new Exception("Cannot read variable '" . $name. "' because it wasn't defined");
		}

		$variable = $this->getVariable($name);
		if (!$variable->isInitialized()) {
			throw new Exception("Variable '" . $name . "' can't be used because is not initialized");
		}

		$variable->increaseUses();
		return $variable;
	}

	/**
	 * Return a variable in the symbol table, it will be used for a write operation
	 *
	 * @return \Variable
	 */
	public function getVariableForWrite($name)
	{

		if (!$this->hasVariable($name)) {
			throw new Exception("Cannot write variable '" . $name. "' because it wasn't defined");
		}

		$variable = $this->getVariable($name);
		$variable->increaseUses();

		return $variable;
	}

	/**
	 * Return a variable in the symbol table, it will be used for a write operation
	 *
	 * @return \Variable
	 */
	public function mustGrownStack($mustGrownStack)
	{
		$this->_mustGrownStack = $mustGrownStack;
	}

}