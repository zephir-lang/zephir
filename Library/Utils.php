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
 * Utils
 *
 * Utility functions
 */
class Utils
{

	/**
	 * Prepares a string to be used as a C-string
	 *
	 * @param       string $str
	 * @return      string
	 */
	public static function addSlaches($str)
	{
		//$str = str_replace('\\', "\\\\", $str);
		$str = str_replace('"', "\\\"", $str);
		$str = str_replace("\n", "\\n", $str);
		$str = str_replace("\r", "\\r", $str);
		$str = str_replace("\t", "\\t", $str);
		return $str;
	}

	/**
	 * Camelizes a string
	 *
	 * @param       string $str
	 * @return      string
	 */
	static public function camelize($str) {
		return str_replace(' ', '', ucwords(str_replace('_', ' ', $str)));
	}

}
