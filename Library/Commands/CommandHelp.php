<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

namespace Zephir\Commands;

use Zephir\Config;
use Zephir\Logger;
use Zephir\Compiler;
use Zephir\Bootstrap;

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
     * Returns the description of the command
     *
     * @return string
     */
    public function getDescription()
    {
        return 'Displays this help';
    }

    /**
     * Executes the command
     * @param Config $config
     * @param Logger $logger
     */
    public function execute(Config $config, Logger $logger)
    {
        echo self::LOGO, PHP_EOL;
        echo "Zephir version " , Compiler::VERSION,  PHP_EOL, PHP_EOL;
        echo "Usage: ", PHP_EOL;
        echo "\tcommand [options]", PHP_EOL;
        echo PHP_EOL;
        echo "Available commands:", PHP_EOL;
        foreach (Bootstrap::getCommands() as $command) {
            echo sprintf("\t%-20s%s\n", $command->getUsage(), $command->getDescription());
        }
        echo PHP_EOL;
        echo "Options:", PHP_EOL;
        echo sprintf("\t%-20s%s\n", "-f([a-z0-9\-]+)", "Enables compiler optimizations");
        echo sprintf("\t%-20s%s\n", "-fno-([a-z0-9\-]+)", "Disables compiler optimizations");
        echo sprintf("\t%-20s%s\n", "-w([a-z0-9\-]+)", "Turns a warning on");
        echo sprintf("\t%-20s%s\n", "-W([a-z0-9\-]+)", "Turns a warning off");
        echo PHP_EOL;
    }
}
