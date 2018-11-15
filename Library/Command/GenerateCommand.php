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
 * Zephir\Command\GenerateCommand
 *
 * Generates C code from the Zephir code without compiling it.
 *
 * @package Zephir\Command
 */
class GenerateCommand extends ContainerAwareCommand
{
    protected function configure()
    {
        $this
            ->setName('generate')
            ->setDescription('Generates C code from the Zephir code without compiling it')
            ->addOption(
                'backend',
                null,
                InputOption::VALUE_REQUIRED,
                'Used backend to generate extension',
                'ZendEngine3'
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
        $this->compiler->generate(true);

        return 0;
    }
}
