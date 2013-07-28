<?php

/**
 * Variable
 *
 * This represents a variable in a symbol table
 */
class Variable
{
	/**
	 * Variable's type
	 */
	protected $_type;

	/**
	 * Variable's name
	 */
	protected $_name;

	/**
	 * Number of times the variable has been read
	 */
	protected $_numberUses = 0;

	/**
	 * Number of mutations to the variable
	 */
	protected $_numberMutates = 0;

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

	/**
	 * Returns variable's name
	 */
	public function getName()
	{
		return $this->_name;
	}

	/**
	 * Increase the number of uses a variable may have
	 */
	public function increaseUses()
	{
		$this->_numberUses++;
	}

	/**
	 * Increase the number of mutations a variable may have
	 */
	public function increaseMutates()
	{
		$this->_numberMutates++;
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
	 * Returns the number of mutations performed over the variable
	 *
	 * @return long
	 */
	public function getNumberMutations()
	{
		return $this->_numberMutates;
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
	 * Set if the variable must be initialized to null
	 *
	 * @param boolean $mustInitNull
	 * @return boolean
	 */
	public function setMustInitNull($mustInitNull)
	{
		$this->_mustInitNull = $mustInitNull;
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

	/**
	 * Observes a variable in the memory frame without initialization
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function observeVariant(CompilationContext $compilationContext)
	{
		if ($this->getName() != 'this') {
			$compilationContext->headersManager->add('kernel/memory');
			$compilationContext->symbolTable->mustGrownStack(true);
			if ($this->_variantInits > 0 || $compilationContext->insideCycle) {
				$this->_mustInitNull = true;
				$compilationContext->codePrinter->output('ZEPHIR_OBS_NVAR(' . $this->getName() . ');');
			} else {
				$compilationContext->codePrinter->output('ZEPHIR_OBS_VAR(' . $this->getName() . ');');
			}
		}
		$this->_variantInits++;
	}

}