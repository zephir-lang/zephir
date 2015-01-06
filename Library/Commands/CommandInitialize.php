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

/**
 * CommandInitialize
 *
 * Initialize a zephir extension
 */
class CommandInitialize extends CommandAbstract
{

    /**
     * Command provided by this command
     *
     * @return string
     */
    public function getCommand()
    {
        return 'init';
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
     * Returns the description of the command
     *
     * @return string
     */
    public function getDescription()
    {
        return 'Initializes a Zephir extension';
    }

    public function execute(Config $config, Logger $logger)
    {
        if (isset($_SERVER['argv'][2])) {
            $this->setParameter('namespace', strtolower(preg_replace('/[^0-9a-zA-Z]/', '', $_SERVER['argv'][2])));
        }
        parent::execute($config, $logger);
    }
}
