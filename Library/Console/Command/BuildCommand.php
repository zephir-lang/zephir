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

use Symfony\Component\Console\Input\ArrayInput;
use Symfony\Component\Console\Input\InputDefinition;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Style\SymfonyStyle;

/**
 * Generates/Compiles/Installs a Zephir extension.
 */
final class BuildCommand extends AbstractCommand
{
    use DevelopmentModeAwareTrait;
    use ZflagsAwareTrait;

    protected function configure()
    {
        $this
            ->setName('build')
            ->setDescription('Generates/Compiles/Installs a Zephir extension')
            ->setDefinition($this->createDefinition())
            ->addOption('dev', null, InputOption::VALUE_NONE, 'Build the extension in development mode')
            ->addOption('no-dev', null, InputOption::VALUE_NONE, 'Build the extension in production mode')
            ->setHelp($this->getDevelopmentModeHelp().PHP_EOL.$this->getZflagsHelp());
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        // call generate
        $command = $this->getApplication()->find('generate');
        $io = new SymfonyStyle($input, $output);

        $arguments = [
            'command' => 'generate',
        ];

        try {
            $command->run(new ArrayInput($arguments), $output);
        } catch (\Exception $e) {
            $io->getErrorStyle()->error($e->getMessage());

            return 1;
        }

        // call compile
        $command = $this->getApplication()->find('compile');
        $io = new SymfonyStyle($input, $output);

        $arguments = [
            'command' => 'compile',
            '--dev' => $this->isDevelopmentModeEnabled($input),
        ];

        try {
            $command->run(new ArrayInput($arguments), $output);
        } catch (\Exception $e) {
            $io->getErrorStyle()->error($e->getMessage());

            return 1;
        }

        // call install
        $command = $this->getApplication()->find('install');
        $io = new SymfonyStyle($input, $output);

        $arguments = [
            'command' => 'install',
            '--dev' => $this->isDevelopmentModeEnabled($input),
        ];

        try {
            return $command->run(new ArrayInput($arguments), $output);
        } catch (\Exception $e) {
            $io->getErrorStyle()->error($e->getMessage());

            return 1;
        }
    }

    protected function createDefinition(): InputDefinition
    {
        return new InputDefinition([]);
    }

    /**
     * @return string
     */
    protected function getDevelopmentModeHelp(): string
    {
        return <<<'EOT'
This is a meta command that just calls the <info>generate</info>, <info>compile</info> and <info>install</info> commands.

Using <comment>--dev</comment> option will force building and installing the extension in development mode
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
