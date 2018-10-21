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
 * Zephir\Commands\CommandInitialize
 *
 * Initializes a Zephir extension
 *
 * @package Zephir\Commands
 */
class CommandInitialize extends CommandAbstract
{
    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getCommand()
    {
        return 'init';
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getDescription()
    {
        return 'Initializes a Zephir extension';
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

        if (isset($_SERVER['argv'][2])) {
            $this->setParameter(
                'namespace',
                strtolower(preg_replace('/[^0-9a-zA-Z]/', '', $_SERVER['argv'][2]))
            );
        }

        parent::execute($config, $logger);
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getUsage()
    {
        $template =<<<EOL
%s [options]

Description:
    The options are as follows:

    namespace                                The extension namespace.
    --backend=ZendEngine2|ZendEngine3        Used backend to create extension.
EOL;

        return sprintf($template, $this->getCommand());
    }
}
