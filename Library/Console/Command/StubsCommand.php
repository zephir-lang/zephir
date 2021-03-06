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

use Symfony\Component\Console\Input\InputDefinition;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Style\SymfonyStyle;
use Zephir\Compiler;
use Zephir\Exception\ExceptionInterface;

/**
 * Zephir\Console\Command\StubsCommand.
 *
 * Generates stubs that can be used in a PHP IDE.
 */
final class StubsCommand extends Command
{
    use ZflagsAwareTrait;

    private $compiler;

    public function __construct(Compiler $compiler)
    {
        $this->compiler = $compiler;

        parent::__construct();
    }

    protected function configure()
    {
        $this
            ->setName('stubs')
            ->setDescription('Generates stubs that can be used in a PHP IDE')
            ->setDefinition($this->createDefinition())
            ->setHelp(sprintf('%s.', $this->getDescription()).PHP_EOL.PHP_EOL.$this->getZflagsHelp());
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $io = new SymfonyStyle($input, $output);

        try {
            // TODO: Move all the stuff from the compiler
            $this->compiler->stubs();
        } catch (ExceptionInterface $e) {
            $io->getErrorStyle()->error($e->getMessage());

            return 1;
        }

        return 0;
    }

    protected function createDefinition()
    {
        return new InputDefinition(
            [
                new InputOption(
                    'backend',
                    null,
                    InputOption::VALUE_REQUIRED,
                    'Used backend to generate stubs for the extension',
                    'ZendEngine3'
                ),
            ]
        );
    }
}
