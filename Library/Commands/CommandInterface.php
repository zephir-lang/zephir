<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
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
