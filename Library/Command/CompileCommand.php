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
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Zephir\Command\CompileCommand
 *
 * Compile a Zephir extension.
 *
 * @package Zephir\Command
 */
class CompileCommand extends ContainerAwareCommand
{
    protected function configure()
    {
        $this
            ->setName('compile')
            ->setDescription('Compile a Zephir extension')
            ->addOption(
                'backend',
                null,
                InputOption::VALUE_REQUIRED,
                'Used backend to compile extension',
                'ZendEngine3'
            )
            ->addOption(
                'dev',
                null,
                InputOption::VALUE_NONE,
                'Compile the extension in development mode'
            );
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        /** TODO: Process flags
        -f([a-z0-9\-]+)     Enables compiler optimizations
        -fno-([a-z0-9\-]+)  Disables compiler optimizations
        -w([a-z0-9\-]+)     Turns a warning on
        -W([a-z0-9\-]+)     Turns a warning off
         */

        // TODO: Move all the stuff from the compiler
        $this->compiler->compile($input->getOption('dev'));

        return 0;
    }
}
