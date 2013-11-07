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
 * CommandHelp
 *
 * Shows compiler help
 */
class CommandHelp extends CommandAbstract
{
	const LOGO ='
 _____              __    _
/__  /  ___  ____  / /_  (_)____
  / /  / _ \/ __ \/ __ \/ / ___/
 / /__/  __/ /_/ / / / / / /
/____/\___/ .___/_/ /_/_/_/
         /_/
';

	/**
	 * Command provided by this command
	 *
	 * @return string
	 */
	public function getCommand()
	{
		return 'help';
	}

	/**
	 * Command usage
	 *
	 * @return string
	 */
	public function getUsage()
	{
		return 'help';
	}

	/**
	 * @return string
	 */
	public function getDescription()
	{
		return 'Displays this help';
	}

	/**
	 * Executes the command
	 *
	 * Config $config
	 * Logger $logger
	 */
	public function execute(Config $config, Logger $logger)
	{
		echo self::LOGO, PHP_EOL;
		echo "zephir version " , CommandVersion::VERSION,  PHP_EOL, PHP_EOL;
		echo "Usage: ", PHP_EOL;
		echo "\tcommand [options]", PHP_EOL;
		echo PHP_EOL;
		echo "Available commands:", PHP_EOL;
		foreach (Bootstrap::getCommands() as $command) {
			echo sprintf("\t%-20s%s\n", $command->getUsage(), $command->getDescription());
		}
		echo PHP_EOL;
		echo "Options:", PHP_EOL;
		echo sprintf("\t%-20s%s\n", "-fno-([a-z0-9\-]+)", "Setting options to Compiler");
		echo sprintf("\t%-20s%s\n", "-W([a-z0-9\-]+)", "Setting warning options to Compiler");
		echo PHP_EOL;
	}
}
