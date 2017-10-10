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
     * on the command line else null
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

    /**
     * Gets currently initialized Command Manager.
     *
     * @return Manager
     */
    public function getCommandsManager();
}
