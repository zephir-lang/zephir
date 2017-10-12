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
 * Zephir\Commands\BuildCommand
 *
 * Generates/Builds/Installs a Zephir extension
 *
 * @package Zephir\Commands
 */
class CommandBuild extends CommandAbstract
{
    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getCommand()
    {
        return 'build';
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getDescription()
    {
        return 'Generates/Builds/Installs a Zephir extension';
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
