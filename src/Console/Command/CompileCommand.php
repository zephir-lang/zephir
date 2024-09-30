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

use const PHP_EOL;

/**
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

    protected function configure(): void
    {
        $this
            ->setName('compile')
            ->setDescription('Compile a Zephir extension')
            ->setDefinition($this->createDefinition())
            ->addOption('dev', null, InputOption::VALUE_NONE, 'Compile the extension in development mode')
            ->addOption('no-dev', null, InputOption::VALUE_NONE, 'Compile the extension in production mode')
            ->addOption('jobs', 'j', InputOption::VALUE_REQUIRED, 'Set make -j (job slots)')
            ->setHelp($this->getDevelopmentModeHelp() . PHP_EOL . $this->getZflagsHelp())
        ;
    }

    protected function createDefinition(): InputDefinition
    {
        return new InputDefinition([]);
    }

    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        $io = new SymfonyStyle($input, $output);

        $jobs = $input->hasOption('jobs') ? (int)$input->getOption('jobs') : null;

        try {
            // TODO: Move all the stuff from the compiler
            $this->compiler->compile(
                $this->isDevelopmentModeEnabled($input),
                $jobs
            );
        } catch (CompilerException $e) {
            $io->getErrorStyle()->error($e->getMessage());

            return 1;
        }

        return 0;
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
