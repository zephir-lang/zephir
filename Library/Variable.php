<?php

/**
 * Variable
 *
 * This represents a variable in a symbol table
 */
class Variable
{
	protected $_type;

	protected $_name;

	protected $_numberUses = 0;

	protected $_initialized = false;

	protected $_variantInits = 0;

	public function __construct($type, $name)
	{
		$this->_type = $type;
		$this->_name = $name;
	}

	public function getType()
	{
		return $this->_type;
	}

	public function getName()
	{
		return $this->_name;
	}

	public function increaseUses()
	{
		$this->_numberUses++;
	}

	public function getNumberUses()
	{
		return $this->_numberUses;
	}

	public function setIsInitialized($initialized)
	{
		$this->_initialized = $initialized;
	}

	/**
	 * Check if the variable is initialized or not
	 *
	 * @return boolean
	 */
	public function isInitialized()
	{
		return $this->_initialized;
	}

	/**
	 * Set if the symbol is a parameter of the method or not
	 *
	 * @param boolean $isExternal
	 */
	public function setIsExternal($isExternal)
	{
		$this->_isExternal = $isExternal;
		$this->_variantInits = 1;
	}

	/**
	 * Initializes a variant variable
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function initVariant(CompilationContext $compilationContext)
	{
		$compilationContext->symbolTable->mustGrownStack(true);
		if ($this->_variantInits > 0) {
			$compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(' . $this->getName() . ');');
		} else {
			$compilationContext->codePrinter->output('ZEPHIR_INIT_VAR(' . $this->getName() . ');');
		}
		$this->_variantInits++;
	}

}