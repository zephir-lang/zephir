<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Console\Command;

use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Zephir\Console\Command\CleanCommand
 *
 * Cleans any object files created by the extension.
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
        $this->filesystem->clean();

        if ($this->environment->isWindows()) {
            system('cd ext && nmake clean-all');
        } else {
            system('cd ext && make clean > /dev/null');
        }

        return 0;
    }
}
