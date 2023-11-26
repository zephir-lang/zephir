<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Console\Command;

use Symfony\Component\Console\Command\ListCommand as Command;

/**
 * List Command
 *
 * Displays the list of all available commands for the application.
 */
final class ListCommand extends Command
{
    use RemoveOptionsTrait;

    /**
     * {@inheritdoc}
     */
    protected function configure(): void
    {
        parent::configure();

        $this->setHidden(true);
    }

    /**
     * {@inheritdoc}
     *
     * @param bool $mergeArgs
     */
    public function mergeApplicationDefinition($mergeArgs = true): void
    {
        parent::mergeApplicationDefinition($mergeArgs);

        $this->removeOptions(['dumpversion', 'version']);
    }
}
