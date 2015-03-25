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
 * CommandInterface
 *
 * Provides an interface to build commands
 */
interface CommandInterface
{

    /**
     * Command provided by this command
     *
     * @return string
     */
    public function getCommand();

    /**
     * Command usage
     *
     * @return string
     */
    public function getUsage();

    /**
     * @return string
     */
    public function getDescription();

    /**
     * Returns parameter named parameterName if specified
     * on the commmand line else null
     * @param string $parameterName
     * @return string
     */
    public function getParameter($parameterName);

    /**
     * Executes the command
     * @param Config $config
     * @param Logger $logger
     */
    public function execute(Config $config, Logger $logger);
}
