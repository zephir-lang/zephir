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

	protected $_tempVariables = array();

	protected $_localContext;

	/**
	 * Variable
	 *
	 * @param \CompilationContext $compilationContext
	 */
	public function __construct($compilationContext)
	{
		/* this_ptr */
		$thisVar = new Variable('variable', 'this', $compilationContext->currentBranch);
		$thisVar->setIsInitialized(true);
		$thisVar->increaseUses();
		$thisVar->setReadOnly(true);
		$thisVar->setLowName('this_ptr');
		$thisVar->setDynamicType('object');
		$this->_variables['this'] = $thisVar;

		$returnValue = new Variable('variable', 'return_value', $compilationContext->currentBranch);
		$returnValue->setIsInitialized(true);
		$returnValue->increaseUses();
		$this->_variables['return_value'] = $returnValue;
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
	public function addVariable($type, $name, CompilationContext $compilationContext, $defaultValue=null)
	{
		$variable = new Variable($type, $name, $compilationContext->currentBranch, $defaultValue);
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

	/**
	 * Checks if a variable is a superglobal
	 *
	 * @param string $name
	 */
	public function isSuperGlobal($name)
	{
		return $name == '_GET' || $name == '_POST' || $name == '_COOKIE' || $name == '_SERVER' || $name == '_SESSION';
	}

	/**
	 * Return a variable in the symbol table, it will be used for a read operation
	 *
	 * @param string $name
	 * @param \CompilationContext $compilationContext
	 * @return \Variable
	 */
	public function getVariableForRead($name, $compilationContext=null, $statement=null)
	{

		/**
		 * Validate that 'this' cannot be used in a static function
		 */
		if ($name == 'this' || $name == 'this_ptr') {
			if ($compilationContext->staticContext) {
				throw new CompilerException("Cannot use variable 'this' in static context", $statement);
			}
		}

		/**
		 * Create superglobals just in time
		 */
		if ($this->isSuperGlobal($name)) {

			if (!$this->hasVariable($name)) {

				/**
				 * @TODO, injecting globals, initialize to null and check first?
				 */
				$compilationContext->codePrinter->output('zephir_get_global(&' . $name . ', SS("' . $name . '") TSRMLS_CC);');

				$superVar = new Variable('variable', $name, $compilationContext->currentBranch);
				$superVar->setIsInitialized(true);
				$superVar->setDynamicType('array');
				$this->_variables[$name] = $superVar;
			}
		}

		if (!$this->hasVariable($name)) {
			throw new CompilerException("Cannot read variable '" . $name . "' because it wasn't declared", $statement);
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
	 * @param CompilationContext $compilationContext
	 * @return \Variable
	 */
	public function getVariableForWrite($name, $compilationContext, $statement=null)
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

				$superVar = new Variable('variable', $name, $compilationContext->currentBranch);
				$superVar->setIsInitialized(true);
				$superVar->setDynamicType('array');
				$this->_variables[$name] = $superVar;
				return $superVar;
			}
		}

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
	 * Register a variable as temporal
	 *
	 * @param string $type
	 * @param string $location
	 * @param \Variable $variable
	 */
	protected function _registerTempVariable($type, $location, Variable $variable)
	{
		if (!isset($this->_tempVariables[$location][$type])) {
			$this->_tempVariables[$location][$type] = array();
		}
		$this->_tempVariables[$location][$type][] = $variable;
	}

	/**
	 * Reuse variables marked as idle after leave a branch
	 *
	 * @param string $type
	 * @param string $location
	 * @return \Variable
	 */
	protected function _reuseTempVariable($type, $location)
	{
		if (isset($this->_tempVariables[$location][$type])) {
			foreach ($this->_tempVariables[$location][$type] as $variable) {
				if (!$variable->isDoublePointer()) {
					if ($variable->isIdle()) {
						$variable->setIdle(false);
						return $variable;
					}
				}
			}
		}
		return null;
	}

	/**
	 * Returns a temporal variable
	 *
	 * @param string $type
	 * @return Variable
	 */
	public function getTempVariable($type, $compilationContext)
	{
		$tempVar = $this->_tempVariable++;
		return $this->addVariable($type, '_' . $tempVar, $compilationContext);
	}

	/**
	 * Creates a temporary variable to be used in a write operation
	 *
	 * @param string $type
	 * @param \CompilationContext $context
	 */
	public function getTempVariableForWrite($type, CompilationContext $context)
	{
		$variable = $this->_reuseTempVariable($type, 'heap');
		if (is_object($variable)) {
			$variable->increaseUses();
			$variable->increaseMutates();
			if ($type == 'variable') {
				$variable->initVariant($context);
			}
			return $variable;
		}

		$tempVar = $this->_tempVariable++;
		$variable = $this->addVariable($type, '_' . $tempVar, $context);
		$variable->setIsInitialized(true);
		$variable->setTemporal(true);
		$variable->increaseUses();
		$variable->increaseMutates();
		if ($type == 'variable') {
			$variable->initVariant($context);
		}

		$this->_registerTempVariable($type, 'heap', $variable);
		return $variable;
	}

	/**
	 * Creates a temporary variable to be used in a write operation
	 * the body of the variable is freed between iterations instead of
	 * request a new full zval variable
	 *
	 * @param string $type
	 * @param \CompilationContext $context
	 */
	public function getTempComplexLiteralVariableForWrite($type, CompilationContext $context)
	{
		$variable = $this->_reuseTempVariable($type, 'heap-literal');
		if (is_object($variable)) {
			$variable->increaseUses();
			$variable->increaseMutates();
			if ($type == 'variable') {
				$variable->initComplexLiteralVariant($context);
			}
			return $variable;
		}

		$tempVar = $this->_tempVariable++;
		$variable = $this->addVariable($type, '_' . $tempVar, $context);
		$variable->setIsInitialized(true);
		$variable->increaseUses();
		$variable->increaseMutates();
		$variable->setTemporal(true);
		if ($type == 'variable') {
			$variable->initComplexLiteralVariant($context);
		}

		$this->_registerTempVariable($type, 'heap-literal', $variable);
		return $variable;
	}

	/**
	 * Creates a temporary variable to be used in a write operation
	 *
	 * @param string $type
	 * @param \CompilationContext $context
	 * @return \Variable
	 */
	public function getTempLocalVariableForWrite($type, CompilationContext $context)
	{

		$variable = $this->_reuseTempVariable($type, 'stack');
		if (is_object($variable)) {
			$variable->increaseUses();
			$variable->increaseMutates();
			$variable->setLocalOnly(true);
			if ($type == 'variable') {
				$variable->initVariant($context);
			}
			return $variable;
		}

		$tempVar = $this->_tempVariable++;
		$variable = $this->addVariable($type, '_' . $tempVar, $context);
		$variable->setIsInitialized(true);
		$variable->increaseUses();
		$variable->increaseMutates();
		$variable->setLocalOnly(true);
		$variable->setTemporal(true);
		if ($type == 'variable') {
			$variable->initVariant($context);
		}

		$this->_registerTempVariable($type, 'stack', $variable);
		return $variable;
	}

	/**
	 * Creates a temporary variable
	 *
	 * @param string $type
	 * @param \CompilationContext $context
	 */
	public function addTemp($type, CompilationContext $context)
	{
		/*$variable = $this->_reuseTempVariable($type, 'heap');
		if (is_object($variable)) {
			$variable->increaseUses();
			$variable->increaseMutates();
			return $variable;
		}*/

		$tempVar = $this->_tempVariable++;
		$variable = $this->addVariable($type, '_' . $tempVar, $context);
		$variable->setIsInitialized(true);
		$variable->setTemporal(true);
		$variable->increaseUses();
		$variable->increaseMutates();

		//$this->_registerTempVariable($type, 'heap', $variable);
		return $variable;
	}

	/**
	 * Creates a temporary variable to be used as intermediate variable of a read operation
	 * Variables are automatically tracked by the memory manager
	 *
	 * @param string $type
	 * @param \CompilationContext $context
	 * @return \Variable
	 */
	public function getTempVariableForObserve($type, CompilationContext $context)
	{

		$variable = $this->_reuseTempVariable($type, 'observe');
		if (is_object($variable)) {
			$variable->increaseUses();
			$variable->increaseMutates();
			$variable->observeVariant($context);
			return $variable;
		}

		$tempVar = $this->_tempVariable++;
		$variable = $this->addVariable($type, '_' . $tempVar, $context);
		$variable->setIsInitialized(true);
		$variable->setTemporal(true);
		$variable->increaseUses();
		$variable->increaseMutates();
		$variable->observeVariant($context);

		$this->_registerTempVariable($type, 'observe', $variable);
		return $variable;
	}

	/**
	 * Returns the temporal variables declared in a given context
	 *
	 * @return array
	 */
	public function getTemporalVariables()
	{
		return $this->_tempVariables;
	}

	/**
	 * Traverses temporal variables created in a specific branch
	 * marking them as idle
	 *
	 * @param \CompilationContext $context
	 */
	public function markTemporalVariablesIdle(CompilationContext $compilationContext)
	{
		$branch = $compilationContext->currentBranch;
		foreach ($this->_tempVariables as $location => $typeVariables) {
			foreach ($typeVariables as $type => $variables) {
				foreach ($variables as $variable) {
					if ($branch == $variable->getBranch()) {
						$variable->setIdle(true);
					}
				}
			}
		}
	}

}
