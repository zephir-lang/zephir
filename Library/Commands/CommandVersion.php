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
use Zephir\Compiler;

/**
 * Zephir\Commands\CommandVersion
 *
 * Shows the Zephir version
 *
 * @package Zephir\Commands
 */
class CommandVersion extends CommandAbstract
{
    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getCommand()
    {
        return 'version';
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
        return 'Shows the Zephir version';
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
            echo $this->getSynopsis();
            return;
        }

        echo Compiler::getCurrentVersion(), PHP_EOL;
    }
}
