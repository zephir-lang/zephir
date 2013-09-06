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
 * Config
 *
 * Manages compiler global configuration
 */
class Config
{
	protected $_config = array(
		'static-type-inference' => true,
		'static-type-inference-second-pass' => true,
		'local-context-pass' => true,
		'constant-folding' => true
	);

	public function __construct()
	{
		if (file_exists('config.json')) {
			$config = json_decode(file_get_contents('config.json'), true);
			$this->_config = array_merge($this->_config, $config);
		}
	}

	public function get($key)
	{
		if (isset($this->_config[$key])) {
			return $this->_config[$key];
		}
		return null;
	}

	public function set($key, $value)
	{
		$this->_config[$key] = $value;
	}

}
