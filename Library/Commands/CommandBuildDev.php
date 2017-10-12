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

/**
 * Zephir\Commands\BuildDevCommand
 *
 * Generates/Builds/Installs a Zephir extension in development mode (development mode)
 *
 * @package Zephir\Commands
 */
class CommandBuildDev extends CommandAbstract
{
    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getCommand()
    {
        return 'builddev';
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getDescription()
    {
        return 'Generates/Builds/Installs a Zephir extension in development mode';
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

    --backend=ZendEngine2|ZendEngine3        The extension namespace.
EOL;

        return sprintf($template, $this->getCommand());
    }
}
