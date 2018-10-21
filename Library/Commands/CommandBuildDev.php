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

    --backend=ZendEngine2|ZendEngine3        Used backend to build extension.
EOL;

        return sprintf($template, $this->getCommand());
    }
}
