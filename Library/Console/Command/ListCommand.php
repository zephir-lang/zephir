<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Console\Command;

use Symfony\Component\Console\Command\ListCommand as Command;

/**
 * Zephir\Console\Command\ListCommand.
 *
 * Displays the list of all available commands for the application.
 */
final class ListCommand extends Command
{
    /**
     * {@inheritdoc}
     */
    protected function configure()
    {
        parent::configure();

        $this->setHidden(true);
    }
}
