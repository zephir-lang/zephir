<?php

class Variable 
{
	protected $_type;

	protected $_name;	

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
}