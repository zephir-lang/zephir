<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
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
 * Provides a common interface for any buil-in command.
 *
 * @package Zephir\Commands
 */
interface CommandInterface
{
    /**
     * Returns command provided by this command.
     *
     * @return string
     */
    public function getCommand();

    /**
     * Returns command usage.
     *
     * @return string
     */
    public function getUsage();

    /**
     * Returns the description of the command.
     *
     * @return string
     */
    public function getDescription();

    /**
     * Returns command's synopsis.
     *
     * @return string
     */
    public function getSynopsis();

    /**
     * Returns parameter named parameterName if specified on the command line else null.
     *
     * @param string $parameterName
     * @return string|null
     */
    public function getParameter($parameterName);

    /**
     * Executes the command.
     *
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
