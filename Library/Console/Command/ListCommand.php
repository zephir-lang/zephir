<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
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
    use RemoveOptionsTrait;

    /**
     * {@inheritdoc}
     */
    protected function configure()
    {
        parent::configure();

        $this->setHidden(true);
    }

    /**
     * {@inheritdoc}
     *
     * @param bool $mergeArgs
     */
    public function mergeApplicationDefinition($mergeArgs = true)
    {
        parent::mergeApplicationDefinition($mergeArgs);

        $this->removeOptions(['dumpversion', 'version']);
    }
}
