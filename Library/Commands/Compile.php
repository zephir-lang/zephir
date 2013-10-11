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
 * CompileCommand
 *
 * Produce the extension installation
 */
class CompileCommand
{

	/**
	 * Commands provided by this command
	 *
	 * @return array|string
	 */
	public function getCommands()
	{
		return 'compile';
	}

	/**
	 * Command usage
	 *
	 * @return string
	 */
	public function getUsage()
	{
		return 'init [namespace]';
	}

	/**
	 * @return string
	 */
	public function getDescription()
	{
		return 'Initializes a Zephir extension';
	}

	/**
	 * Executes the command
	 *
	 * Config $config
	 * Logger $logger
	 */
	public function execute(Config $config, Logger $logger)
	{
		$compiler = new Compiler();
		$compiler->compile($config, $logger);
		$compiler->install($config, $logger);
	}

}
