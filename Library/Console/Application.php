<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Console;

use Psr\Container\ContainerInterface;
use Symfony\Component\Console\Application as BaseApplication;
use Symfony\Component\Console\Command\Command as BaseCommand;
use Symfony\Component\Console\Input\InputDefinition;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Zephir\Config;
use Zephir\Di\Singleton;
use Zephir\Environment;
use Zephir\Exception\ExceptionInterface;
use Zephir\Providers\CompilerProvider;
use Zephir\ServiceRegistrator;
use Zephir\Zephir;

final class Application extends BaseApplication
{
    /**
     * The ServiceRegistrator instance.
     *
     * @var ServiceRegistrator
     */
    protected $registrator;

    /**
     * Bootstrap constructor.
     *
     * @param string $basePath The Zephir compiler base directory.
     * @param ContainerInterface $container The DI container (if any).
     */
    public function __construct($basePath, ContainerInterface $container = null)
    {
        parent::__construct('Zephir', Zephir::VERSION);

        try {
            $this->registrator = new ServiceRegistrator($basePath, $container);

            $this->registerCompiler();
        } catch (\Exception $e) {
            $container = Singleton::getDefault();

            // TODO: Handle it better by ErrorHandler
            $config = $container && $container->has('config') ? $container->get('config') : null;
            fwrite(
                STDERR,
                $this->formatErrorMessage($e, $config)
            );

            exit(1);
        }
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getHelp()
    {
        return Zephir::LOGO . parent::getHelp();
    }

    /**
     * {@inheritdoc}
     *
     * @param InputInterface $input
     * @param OutputInterface $output
     * @throws \Exception|\Throwable
     * @return int
     */
    public function doRun(InputInterface $input, OutputInterface $output)
    {
        if (true === $input->hasParameterOption(['--dumpversion'], true)) {
            $output->writeln($this->getVersion());

            return 0;
        }

        return parent::doRun($input, $output);
    }

    /**
     * Register Zephir compiler.
     *
     * @return void
     */
    protected function registerCompiler()
    {
        $this->registrator->registerService(
            new CompilerProvider()
        );
    }

    /**
     * {@inheritdoc}
     *
     * @return InputDefinition
     */
    protected function getDefaultInputDefinition()
    {
        $definition = parent::getDefaultInputDefinition();

        $definition->addOption(
            new InputOption(
                'dumpversion',
                null,
                InputOption::VALUE_NONE,
                "Print the Zephir version — and don't do anything else"
            )
        );

        return $definition;
    }

    /**
     * {@inheritdoc}
     *
     * @return BaseCommand[]
     */
    protected function getDefaultCommands()
    {
        $commands = parent::getDefaultCommands();
        $di = $this->registrator->getContainer();

        $commands[] = new Command\InitCommand($di);
        $commands[] = new Command\GenerateCommand($di);
        $commands[] = new Command\CompileCommand($di);
        $commands[] = new Command\InstallCommand($di);
        $commands[] = new Command\BuildCommand($di);
        $commands[] = new Command\CleanCommand($di);
        $commands[] = new Command\FullCLeanCommand($di);
        $commands[] = new Command\ApiCommand($di);
        $commands[] = new Command\StubsCommand($di);

        // TODO
        // if (\substr(__FILE__, 0, 5) === 'phar:') {
        //   $app->add(new Zephir\Command\SelfUpdateCommand());
        // }

        return $commands;
    }

    /**
     * Formats error message to show an exception opening the file and highlighting the wrong part.
     *
     * @todo
     * @deprecated
     *
     * @param \Exception $exception
     * @param Config $config Current config object [optional].
     * @return string
     */
    protected function formatErrorMessage(\Exception $exception, Config $config = null)
    {
        $message = '';

        if ($config && $config->get('verbose')) {
            $message .= sprintf('[%s]: ', get_class($exception));
        }

        $message .= $exception->getMessage() . PHP_EOL;

        if ($exception instanceof ExceptionInterface && $extraInfo = $exception->getErrorRegion()) {
            $message .= sprintf("\n%s", $extraInfo);
        }

        $message .= PHP_EOL;
        if (!$config || !$config->get('verbose')) {
            return $message;
        }

        $container = $this->registrator->getContainer();

        /** @var Environment $environment */
        $environment = $container->has('environment') ? $container->get('environment') : null;

        $preparePaths = function ($path) use ($environment) {
            if ($environment) {
                $path = str_replace($environment->getPath() . DIRECTORY_SEPARATOR, '', $path);
            }

            return $path;
        };

        $message .= sprintf("at %s(%s)\n\n", $preparePaths($exception->getFile()), $exception->getLine());
        $message .= sprintf("Stack trace:\n%s\n", $preparePaths($exception->getTraceAsString()));

        return $message;
    }
}
