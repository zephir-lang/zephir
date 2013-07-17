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

}