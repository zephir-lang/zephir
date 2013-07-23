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

	protected $_isExternal = false;

	protected $_variantInits = 0;

	protected $_mustInitNull = false;

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

	/**
	 * Return the number of uses
	 *
	 * @return long
	 */
	public function getNumberUses()
	{
		return $this->_numberUses;
	}

	/**
	 * Sets if the variable is initialized
	 * This allow to throw an exception if the variable is read without initialization
	 *
	 * @param boolean $initialized
	 */
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
	 * Check if the variable is a parameter
	 *
	 * @return boolean
	 */
	public function isExternal()
	{
		return $this->_isExternal;
	}

	/**
	 * Set if the variable must be initialized to null
	 *
	 * @return boolean
	 */
	public function mustInitNull()
	{
		return $this->_mustInitNull;
	}

	/**
	 * Initializes a variant variable
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function initVariant(CompilationContext $compilationContext)
	{
		if ($this->getName() != 'this') {
			$compilationContext->headersManager->add('kernel/memory');
			$compilationContext->symbolTable->mustGrownStack(true);
			if ($this->_variantInits > 0 || $compilationContext->insideCycle) {
				$this->_mustInitNull = true;
				$compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(' . $this->getName() . ');');
			} else {
				$compilationContext->codePrinter->output('ZEPHIR_INIT_VAR(' . $this->getName() . ');');
			}
		}
		$this->_variantInits++;
	}

}