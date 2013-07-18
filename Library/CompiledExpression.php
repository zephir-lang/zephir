<?php

/**
 * CompiledExpression
 *
 * This represent a compiled expression, the object can be used to check
 * if the expression type is able to be used in certain types of the application
 */
class CompiledExpression
{
	protected $_type;

	protected $_code;

	public function __construct($type, $code)
	{
		$this->_type = $type;
		$this->_code = $code;
	}

	public function getType()
	{
		return $this->_type;
	}

	public function getCode()
	{
		return $this->_code;
	}

}
