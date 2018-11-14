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
use Zephir\FileSystem\HardDisk;

/**
 * Zephir\Command\CleanCommand
 *
 * Cleans any object files created by the extension.
 *
 * @package Zephir\Command
 */
class CleanCommand extends ContainerAwareCommand
{
    protected function configure()
    {
        $this
            ->setName('clean')
            ->setDescription('Cleans any object files created by the extension');
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        // TODO: Replace by service
        $fileSystem = new HardDisk();
        $fileSystem->clean();

        if ($this->environment->isWindows()) {
            system('cd ext && nmake clean-all');
        } else {
            system('cd ext && pwd && make clean > /dev/null');
        }
    }
}
