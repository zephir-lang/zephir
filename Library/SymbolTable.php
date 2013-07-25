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

	protected $_tempVariable = 0;

	public function __construct()
	{
		$thisVar = new Variable('variable', 'this');
		$thisVar->setIsInitialized(true);
		$thisVar->increaseUses();
		$this->_variables['this'] = $thisVar;
	}

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
	 * @param array
	 * @return \Variable
	 */
	public function addVariable($type, $name, $defaultValue=null)
	{
		$variable = new Variable($type, $name, $defaultValue);
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
	public function getVariableForRead($name, $statement=null)
	{

		if (!$this->hasVariable($name)) {
			throw new CompilerException("Cannot read variable '" . $name . "' because it wasn't defined", $statement);
		}

		$variable = $this->getVariable($name);
		if (!$variable->isInitialized()) {
			throw new CompilerException("Variable '" . $name . "' can't be used because is not initialized", $statement);
		}

		$variable->increaseUses();
		return $variable;
	}

	/**
	 * Return a variable in the symbol table, it will be used for a write operation
	 *
	 * @return \Variable
	 */
	public function getVariableForWrite($name, $statement=null)
	{

		if (!$this->hasVariable($name)) {
			throw new CompilerException("Cannot write variable '" . $name. "' because it wasn't defined", $statement);
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

	/**
	 * Returns if the current symbol label must add a memory frame
	 *
	 * @return boolean
	 */
	public function getMustGrownStack()
	{
		return $this->_mustGrownStack;
	}

	/**
	 * Returns a temporal variable
	 *
	 * @return Variable
	 */
	public function getTempVariable($type)
	{
		$tempVar = $this->_tempVariable++;
		return $this->addVariable($type, '_' . $tempVar);
	}

	public function getTempVariableForWrite($type, CompilationContext $context)
	{
		$tempVar = $this->_tempVariable++;
		$variable = $this->addVariable($type, '_' . $tempVar);
		$variable->increaseUses();
		$variable->initVariant($context);
		return $variable;
	}

}
