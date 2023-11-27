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

namespace Zephir\Console;

use Exception;
use Symfony\Component\Console\Application as BaseApplication;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Command\HelpCommand;
use Symfony\Component\Console\ConsoleEvents;
use Symfony\Component\Console\Exception\CommandNotFoundException;
use Symfony\Component\Console\Exception\ExceptionInterface;
use Symfony\Component\Console\Exception\RuntimeException;
use Symfony\Component\Console\Input\ArrayInput;
use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Input\InputDefinition;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\EventDispatcher\EventDispatcher;
use Throwable;
use Zephir\Console\Command\ListCommand;
use Zephir\Zephir;

use function explode;
use function fprintf;
use function implode;
use function sprintf;
use function strtolower;
use function trim;

use const PHP_EOL;
use const STDERR;

final class Application extends BaseApplication
{
    public function __construct()
    {
        parent::__construct('Zephir', Zephir::VERSION);

        $dispatcher = new EventDispatcher();
        $dispatcher->addListener(ConsoleEvents::ERROR, [new ErrorListener(), 'onCommandError']);

        $this->setDispatcher($dispatcher);
    }

    /**
     * {@inheritdoc}
     *
     * @param InputInterface  $input
     * @param OutputInterface $output
     *
     * @return int
     *
     * @throws Exception|Throwable
     */
    public function doRun(InputInterface $input, OutputInterface $output): int
    {
        if (true === $input->hasParameterOption(['--dumpversion', '-dumpversion'], true)) {
            $output->writeln($this->getVersion());

            return 0;
        }

        if (true === $input->hasParameterOption(['--vernum'], true)) {
            $output->writeln($this->getVerNum());

            return 0;
        }

        try {
            // Makes ArgvInput::getFirstArgument() able to distinguish an option from an argument.
            $input->bind($this->getDefinition());
        } catch (ExceptionInterface) {
            // Errors must be ignored, full binding/validation happens later when the command is known.
        }

        $wantsHelp = false;
        $name      = $this->getCommandName($input);

        if ($name && 'help' == strtolower($name) && 2 == $_SERVER['argc']) {
            $wantsHelp = true;
        } elseif (!$name && true === $input->hasParameterOption(['--help', '-h'], true)) {
            $wantsHelp = true;
        }

        if ($wantsHelp) {
            $command = $this->find('list');

            return $command->run(new ArrayInput(['command' => 'list']), $output);
        }

        try {
            return parent::doRun($input, $output);
        } catch (CommandNotFoundException | RuntimeException $e) {
            fprintf(STDERR, $e->getMessage() . PHP_EOL);

            return 1;
        }
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getHelp(): string
    {
        return Zephir::LOGO . parent::getHelp();
    }

    /**
     * {@inheritdoc}
     *
     * @return string The long application version
     */
    public function getLongVersion(): string
    {
        $version = explode('-', $this->getVersion());
        $commit  = "({$version[1]})";

        return trim(
            sprintf(
                '%s <info>%s</info> by <comment>Andres Gutierrez</comment> and <comment>Serghei Iakovlev</comment> %s',
                $this->getName(),
                $version[0],
                $commit
            )
        );
    }

    /**
     * Get Version Number
     *
     * @return string The application version
     */
    public function getVerNum(): string
    {
        $version = explode('-', parent::getVersion());
        $version = explode('.', $version[0]);

        return $version[0] . sprintf('%02s', $version[1]) . sprintf('%02s', $version[2]);
    }

    /**
     * Gets the application version as integer.
     *
     * @return string The application version
     */
    public function getVersion(): string
    {
        $version = explode('-', parent::getVersion());

        if (isset($version[1]) && str_starts_with($version[1], '$')) {
            return "{$version[0]}-source";
        }

        return implode('-', $version);
    }

    /**
     * {@inheritdoc}
     *
     * @return Command[] An array of default Command instances
     */
    protected function getDefaultCommands(): array
    {
        return [new HelpCommand(), new ListCommand()];
    }

    /**
     * {@inheritdoc}
     *
     * @return InputDefinition An InputDefinition instance
     */
    protected function getDefaultInputDefinition(): InputDefinition
    {
        return new InputDefinition([
            new InputArgument('command', InputArgument::REQUIRED, 'The command to execute'),

            new InputOption(
                'dumpversion',
                null,
                InputOption::VALUE_NONE,
                "Print the version of the compiler and don't do anything else (also works with a single hyphen)"
            ),
            new InputOption('--help', '-h', InputOption::VALUE_NONE, 'Print this help message'),
            new InputOption('--no-ansi', null, InputOption::VALUE_NONE, 'Disable ANSI output'),
            new InputOption(
                '--verbose',
                '-v',
                InputOption::VALUE_NONE,
                'Displays more detail in error messages from exceptions generated by commands ' .
                '(can also disable with -V)'
            ),
            new InputOption(
                'vernum',
                null,
                InputOption::VALUE_NONE,
                'Print the version of the compiler as integer'
            ),
            new InputOption('--version', null, InputOption::VALUE_NONE, 'Print compiler version information and quit'),
        ]);
    }
}
