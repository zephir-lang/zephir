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

use Symfony\Component\Console\Input\InputDefinition;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Style\SymfonyStyle;
use Zephir\Compiler;
use Zephir\Exception\CompilerException;

/**
 * Zephir\Console\Command\CompileCommand.
 *
 * Compile a Zephir extension.
 */
final class CompileCommand extends AbstractCommand
{
    use DevelopmentModeAwareTrait;
    use ZflagsAwareTrait;

    private Compiler $compiler;

    public function __construct(Compiler $compiler)
    {
        $this->compiler = $compiler;

        parent::__construct();
    }

    protected function configure()
    {
        $this
            ->setName('compile')
            ->setDescription('Compile a Zephir extension')
            ->setDefinition($this->createDefinition())
            ->addOption('dev', null, InputOption::VALUE_NONE, 'Compile the extension in development mode')
            ->addOption('no-dev', null, InputOption::VALUE_NONE, 'Compile the extension in production mode')
            ->setHelp($this->getDevelopmentModeHelp().PHP_EOL.$this->getZflagsHelp());
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $io = new SymfonyStyle($input, $output);

        try {
            // TODO: Move all the stuff from the compiler
            $this->compiler->compile(
                $this->isDevelopmentModeEnabled($input)
            );
        } catch (CompilerException $e) {
            $io->getErrorStyle()->error($e->getMessage());

            return 1;
        }

        return 0;
    }

    protected function createDefinition(): InputDefinition
    {
        return new InputDefinition(
            [
                new InputOption(
                    'backend',
                    null,
                    InputOption::VALUE_REQUIRED,
                    'Used backend to compile extension',
                    'ZendEngine3'
                ),
            ]
        );
    }

    /**
     * @return string
     */
    protected function getDevelopmentModeHelp(): string
    {
        return <<<EOT
Using <comment>--dev</comment> option will force compiling the extension in development mode
(debug symbols and no optimizations). An extension compiled with debugging symbols means
you can run a program or library through a debugger and the debugger's output will be user
friendlier. These debugging symbols also enlarge the program or library significantly.

NOTE: Zephir development mode will be enabled silently if your PHP binary was compiled in
a debug configuration.

In some cases, we would like to get production ready extension even if the PHP binary was
compiled in a debug configuration. Use <comment>--no-dev</comment> option to achieve this behavior.

EOT;
    }
}
