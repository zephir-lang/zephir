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

	public function __construct(Config $config)
	{
		$this->_config = $config;
	}

	/**
	 *
	 * @param string $message
	 * @param string $type
	 * @param array $node
	 */
	public function warning($message, $type, $node)
	{
		if (!$this->_config->get('silent')) {
			echo 'Warning: ' . $message;
			echo ' in ' . $node['file'] . ' on ' . $node['line'];
			echo ' [' . $type . ']' . PHP_EOL;
			echo PHP_EOL;
			if (!isset($_files[$node['file']])) {
				$lines = file($node['file']);
				$_files[$node['file']] = $lines;
			} else {
				$lines = $_files[$node['file']];
			}
			if (isset($lines[$node['line'] - 1])) {
				$line = $lines[$node['line'] - 1];
				echo "\t", str_replace("\t", " ", $line);
				if (($node['char'] - 1) > 0) {
					echo "\t", str_repeat("-", $node['char'] - 1), "^", PHP_EOL;
				}
			}
		}
	}

}
