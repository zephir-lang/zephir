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

    namespace        The extension namespace.
EOL;

        return sprintf($template, $this->getCommand());
    }
}
