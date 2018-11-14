<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Command;

use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Zephir\Command\InstallCommand
 *
 * Installs the extension in the extension directory.
 *
 * @package Zephir\Command
 */
class InstallCommand extends ContainerAwareCommand
{
    protected function configure()
    {
        $this
            ->setName('install')
            ->setDescription('Installs the extension in the extension directory (may require root password)');
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        // TODO: Move all the stuff from the compiler
        $this->compiler->install();

        return 0;
    }
}
