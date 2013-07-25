<?php

class ParseException extends Exception
{
	protected $_extra;

	public function __construct($message, $extra=null)
	{
		if (is_array($extra)) {
			$message .= " in " . $extra['file'] . " on line " . $extra['line'];
		}
		$this->_extra = $extra;
		parent::__construct($message);
	}

	public function getExtra()
	{
		return $this->_extra;
	}
}