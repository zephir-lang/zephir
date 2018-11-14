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
 * Zephir\Command\StubsCommand
 *
 * Generates stubs that can be used in a PHP IDE.
 *
 * @package Zephir\Command
 */
class StubsCommand extends ContainerAwareCommand
{
    protected function configure()
    {
        $this
            ->setName('stubs')
            ->setDescription('Generates stubs that can be used in a PHP IDE')
            ->addOption(
                'backend',
                null,
                InputOption::VALUE_REQUIRED,
                'Used backend to generate stubs for the extension',
                'ZendEngine3'
            );
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        // TODO: Move all the stuff from the compiler
        $this->compiler->stubs($input->getOption('dev'));

        return 0;
    }
}
