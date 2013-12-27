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
 * Config
 *
 * Manages compiler global configuration
 */
class Config
{
	protected $_config = array(
		'optimizations' => array(
			'static-type-inference' => true,
			'static-type-inference-second-pass' => true,
			'local-context-pass' => true,
			'constant-folding' => true,
			'static-constant-class-folding' => true,
		),
		'namespace'   => '',
		'name'        => '',
		'description' => '',
		'author'      => '',
		'version'	  => '0.0.1'
	);

	protected $_changed = false;

	public function __construct()
	{
		if (file_exists('config.json')) {
			$config = json_decode(file_get_contents('config.json'), true);
			if (!is_array($config)) {
				throw new Exception("config.json is not valid or there is no Zephir extension initialized in this directory");
			}
			$this->_config = array_merge($this->_config, $config);
		}
		register_shutdown_function(array($this, '_saveOnExit'));
	}

	/**
	 * @param $key
	 * @param null $namespace
	 * @return mixed
	 */
	public function get($key, $namespace=null)
	{
		if ($namespace !== null) {
			if (isset($this->_config[$namespace][$key])) {
				return $this->_config[$namespace][$key];
			}
		} else {
			if (isset($this->_config[$key])) {
				return $this->_config[$key];
			}
		}

		return null;
	}

	/**
	 *
	 * @param string $key
	 * @param mixed $value
	 */
	public function set($key, $value)
	{
		$this->_config[$key] = $value;
		$this->_changed = true;
	}

	public function _saveOnExit()
	{
		if ($this->_changed) {
			/**
			 * Above PHP 5.4
			 */
			if (defined('JSON_PRETTY_PRINT')) {
				$config = json_encode($this->_config, JSON_PRETTY_PRINT);
			} else {
				$config = json_encode($this->_config);
			}
			file_put_contents('config.json', $config);
		}
	}
}
