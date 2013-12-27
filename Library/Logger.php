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
 * Logger
 *
 * Entrypoint for warnings/notices/errors generated in compilation
 */
class Logger
{
	private static $_files = array();

	/**
	 * @var Config
	 */
	protected $_config;

	protected $_warnings = array(
		'unused-variable'                    => true,
		'unused-variable-external'           => false,
		'possible-wrong-parameter'           => true,
		'possible-wrong-parameter-undefined' => false,
		'nonexistent-function'               => true,
		'nonexistent-class'                  => true,
		'non-valid-isset'                    => true,
		'non-array-update'                   => true,
		'non-valid-objectupdate'             => true,
		'non-valid-fetch'                    => true,
		'invalid-array-index'                => true,
		'non-array-append'                   => true,
		'invalid-return-type'                => true,
		'unrecheable-code'                   => true,
		'nonexistant-constant'               => true,
		'non-valid-decrement'                => true,
		'non-valid-increment'                => true,
		'non-valid-clone'                    => true,
		'non-array-access'                   => true,
		'invalid-reference'                  => true
	);

	/**
	 * Logger constructor
	 *
	 * @param Config $config
	 */
	public function __construct(Config $config)
	{
		$this->_config = $config;
	}

	/**
	 * Changes a warning status on/off
	 *
	 * @param string $type
	 * @param boolean $value
	 */
	public function set($type, $value)
	{
		$this->_warnings[$type] = $value;
	}

	/**
	 * @param $message
	 * @param $type
	 * @param $node
	 * @return bool
	 * @throws CompilerException
	 */
	public function warning($message, $type, $node)
	{
		if (!$this->_config->get('silent')) {
			if (!isset($this->_warnings[$type])) {
				throw new CompilerException("Unknown warning type: " . $type, $node);
			}

			if (!$this->_warnings[$type]) {
				return false;
			}

			$warning  = 'Warning: ' . $message;
			$warning .= ' in ' . $node['file'] . ' on ' . $node['line'];
			$warning .= ' [' . $type . ']' . PHP_EOL;
			$warning .= PHP_EOL;
			if (!isset($_files[$node['file']])) {
				if (file_exists($node['file'])) {
					$lines = file($node['file']);
				} else {
					$lines = array();
				}
				$_files[$node['file']] = $lines;
			} else {
				$lines = $_files[$node['file']];
			}
			if (isset($lines[$node['line'] - 1])) {
				$line = $lines[$node['line'] - 1];
				$warning .= "\t" . str_replace("\t", " ", $line);
				if (($node['char'] - 1) > 0) {
					$warning .= "\t" . str_repeat("-", $node['char'] - 1) . "^" . PHP_EOL;
				}
			}
			$warning .= PHP_EOL;

			echo Color::warning($warning);

			return true;
		}

		return false;
	}

	/**
	 * @param $message
	 * @return bool
	 */
	public function output($message)
	{
		if (!$this->_config->get('silent')) {
			echo $message;

			return true;
		}

		return false;
	}
}
