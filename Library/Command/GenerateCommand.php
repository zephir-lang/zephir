<?php

/*
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
 */
class GenerateCommand extends ContainerAwareCommand implements ZflagsAwareInterface
{
    use ZflagsAwareTrait;

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
            )
            ->setHelp($this->getZflagsHelp());
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        // TODO: Move all the stuff from the compiler
        $this->compiler->generate(true);

        return 0;
    }
}
