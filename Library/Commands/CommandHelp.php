<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir (https://zephir-lang.com/)             |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */

namespace Zephir\Commands;

use Zephir\Config;
use Zephir\Logger;
use Zephir\Compiler;

/**
 * Zephir\Commands\CommandHelp
 *
 * Shows Zephir help and exit
 *
 * @package Zephir\Commands
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
     * {@inheritdoc}
     *
     * @return string
     */
    public function getCommand()
    {
        return 'help';
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getUsage()
    {
        return $this->getCommand();
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getDescription()
    {
        return 'Displays this help and exit';
    }

    /**
     * {@inheritdoc}
     *
     * @param Config $config
     * @param Logger $logger
     */
    public function execute(Config $config, Logger $logger)
    {
        if ($this->hasHelpOption()) {
            $this->formatUsage();
            return;
        }

        echo self::LOGO, PHP_EOL;
        echo "Zephir version " , Compiler::getCurrentVersion(),  PHP_EOL, PHP_EOL;
        echo "Usage: ", PHP_EOL;
        echo "\tcommand [options]", PHP_EOL;
        echo PHP_EOL;
        echo "Available commands:", PHP_EOL;

        $commands = $this->getCommandsManager();
        $commands->rewind();

        while ($commands->valid()) {
            $command = $commands->current();
            echo sprintf("\t%-20s%s\n", $command->getCommand(), $command->getDescription());

            $commands->next();
        }

        echo PHP_EOL;
        echo "Options:", PHP_EOL;
        echo sprintf("\t%-20s%s\n", '--help|--h', "Displays command help and exit");
        echo sprintf("\t%-20s%s\n", '-f([a-z0-9\-]+)', "Enables compiler optimizations");
        echo sprintf("\t%-20s%s\n", '-fno-([a-z0-9\-]+)', "Disables compiler optimizations");
        echo sprintf("\t%-20s%s\n", '-w([a-z0-9\-]+)', "Turns a warning on");
        echo sprintf("\t%-20s%s\n", '-W([a-z0-9\-]+)', "Turns a warning off");
    }
}
