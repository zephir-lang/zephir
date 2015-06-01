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

use Zephir\CommandArgumentParser;
use Zephir\Config;
use Zephir\Logger;
use Zephir\Compiler;

/**
 * CommandAbstract
 *
 * Provides a superclass for commands
 */
abstract class CommandAbstract implements CommandInterface
{
    private $_parameters = null;

    /**
     * Returns parameter named $name if specified
     * on the command line else null
     *
     * @param string $name
     * @param string $value
     * @return void
     */
    protected function setParameter($name, $value)
    {
        if (!isset($this->_parameters)) {
            $this->_parameters = array();
        }
        $this->_parameters[$name] = $value;
    }

    /**
     * Returns parameter named $name if specified
     * on the command line else null
     * @param string $name
     * @return string
     */
    public function getParameter($name)
    {
        return (isset($this->_parameters[$name])) ? $this->_parameters[$name] : null;
    }


    /**
     * Parse the input arguments for the command and returns theme as an associative array
     * @return array the list of the parameters
     */
    public function parseArguments()
    {

        if (count($_SERVER['argv']) > 2) {
            $commandArgs = array_slice($_SERVER['argv'], 2);
            $parser = new CommandArgumentParser();
            $params = $parser->parseArgs(array_merge(array("command"), $commandArgs));
        } else {
            $params = array();
        }

        return $params;

    }

    /**
     * Executes the command
     * @param Config $config
     * @param Logger $logger
     */
    public function execute(Config $config, Logger $logger)
    {
        $compiler = new Compiler($config, $logger);
        $command = $this->getCommand();
        $compiler->$command($this);
    }
}
