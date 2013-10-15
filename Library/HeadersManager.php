<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

/**
 * Class HeadersManager
 *
 * Manages the c-headers that must be added to a file
 */
class HeadersManager
{
	/**
	 * List of headers
	 * @var array
	 */
	protected $_headers = array();

	/**
	 * @param string $path
	 */
	public function add($path)
	{
		if (!is_string($path)) {
			throw new \InvalidArgumentException('$path must be only string type');
		}
		$this->_headers[$path] = $path;
	}

	/**
	 * @return array
	 */
	public function get()
	{
		return $this->_headers;
	}
}
