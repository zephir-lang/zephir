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
 * InstallCommand
 *
 * Produce the extension installation
 */
class InstallCommand
{

	/**
	 * Commands provided by this command
	 *
	 * @return array|string
	 */
	public function getCommands()
	{
		return 'install';
	}

	/**
	 * Command usage
	 *
	 * @return string
	 */
	public function getUsage()
	{
		return 'install';
	}

	/**
	 * @return string
	 */
	public function getDescription()
	{
		return 'Installs the extension';
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
		$compiler->install($config, $logger);
	}

}
