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
 * Zephir\Command\CleanCommand
 *
 * Cleans any object files created by the extension
 *
 * @package Zephir\Commands
 */
class CommandClean extends CommandAbstract
{
    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getCommand()
    {
        return 'clean';
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
        return 'Cleans any object files created by the extension';
    }
}
