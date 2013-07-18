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

	public function setInitialized($initialized)
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

	public function initVariant(CodePrinter $codePrinter)
	{
		$this->_variantInits++;
	}

}