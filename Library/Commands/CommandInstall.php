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
 * Zephir\Commands\CommandInstall
 *
 * Installs the extension in the extension directory
 *
 * @package Zephir\Commands
 */
class CommandInstall extends CommandAbstract
{
    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getCommand()
    {
        return 'install';
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
        return 'Installs the extension in the extension directory (may require root password)';
    }
}
