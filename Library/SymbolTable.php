<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

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

	protected $_localContext;

	public function __construct()
	{
		$thisVar = new Variable('variable', 'this');
		$thisVar->setIsInitialized(true);
		$thisVar->increaseUses();
		$thisVar->setReadOnly(true);
		$thisVar->setLowName('this_ptr');
		$this->_variables['this'] = $thisVar;
	}

	/**
	 * Sets the local context information
	 *
	 * @param LocalContextPass $localContext
	 */
	public function setLocalContext(LocalContextPass $localContext)
	{
		$this->_localContext = $localContext;
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
		if ($type == 'variable') {
			if ($this->_localContext) {
				/**
				 * Checks whether a variable can be optimized to be static or not
				 */
				if ($this->_localContext->shouldBeLocal($name)) {
					$variable->setLocalOnly(true);
				}
			}
		}
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

	public function isSuperGlobal($name)
	{
		return $name == '_GET' || $name == '_POST' || $name == '_COOKIE' || $name == '_SERVER' || $name == '_SESSION';
	}

	/**
	 * Return a variable in the symbol table, it will be used for a read operation
	 *
	 * @return \Variable
	 */
	public function getVariableForRead($name, $compilationContext=null, $statement=null)
	{

		/**
		 * Create superglobals just in time
		 */
		if ($this->isSuperGlobal($name)) {

			if (!$this->hasVariable($name)) {

				/**
				 * @TODO, injecting globals, initialize to null and check first?
				 */
				$compilationContext->codePrinter->output('zephir_get_global(&' . $name . ', SS("' . $name . '") TSRMLS_CC);');

				$superVar = new Variable('variable', $name);
				$superVar->setIsInitialized(true);
				$this->_variables[$name] = $superVar;
			}
		}

		if (!$this->hasVariable($name)) {
			throw new CompilerException("Cannot read variable '" . $name . "' because it wasn't defined", $statement);
		}

		$variable = $this->getVariable($name);
		if (!$variable->isInitialized()) {
			throw new CompilerException("Variable '" . $name . "' can't be used because is not initialized ", $statement);
		}

		$variable->increaseUses();
		return $variable;
	}

	/**
	 * Return a variable in the symbol table, it will be used for a write operation
	 * Some variables aren't writables themselves but their members do
	 *
	 * @param string $name
	 * @param array $statement
	 * @return \Variable
	 */
	public function getVariableForWrite($name, $statement=null)
	{

		if (!$this->hasVariable($name)) {
			throw new CompilerException("Cannot write variable '" . $name . "' because it wasn't defined", $statement);
		}

		$variable = $this->getVariable($name);
		$variable->increaseUses();
		$variable->increaseMutates();

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

	/**
	 * Creates a temporary variable to be used in a write operation
	 *
	 * @param string $type
	 *
	 */
	public function getTempVariableForWrite($type, CompilationContext $context)
	{
		$tempVar = $this->_tempVariable++;
		$variable = $this->addVariable($type, '_' . $tempVar);
		$variable->setIsInitialized(true);
		$variable->increaseUses();
		$variable->increaseMutates();
		$variable->initVariant($context);
		return $variable;
	}

	/**
	 * Creates a temporary hash
	 *
	 * @param string $type
	 *
	 */
	public function addTemp($type, CompilationContext $context)
	{
		$tempVar = $this->_tempVariable++;
		$variable = $this->addVariable($type, '_' . $tempVar);
		$variable->setIsInitialized(true);
		$variable->increaseUses();
		$variable->increaseMutates();
		return $variable;
	}

	/**
	 * Creates a temporary variable to be used as intermediate variable of a read operation
	 * Variables are automatically tracked by the memory manager
	 *
	 * @param string $type
	 *
	 * @return \Variable
	 */
	public function getTempVariableForObserve($type, CompilationContext $context)
	{
		$tempVar = $this->_tempVariable++;
		$variable = $this->addVariable($type, '_' . $tempVar);
		$variable->setIsInitialized(true);
		$variable->increaseUses();
		$variable->increaseMutates();
		$variable->observeVariant($context);
		return $variable;
	}

}
