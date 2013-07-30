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

class CompilerException extends Exception
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