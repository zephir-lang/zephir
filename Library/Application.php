<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir;

use Psr\Container\ContainerInterface;
use Symfony\Component\Console\Application as BaseApplication;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputDefinition;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Zephir\Command\ContainerAwareCommand;
use Zephir\Di\Singleton;
use Zephir\Exception\ExceptionInterface;
use Zephir\Providers\CompilerProvider;

/**
 * Zephir\Bootstrap
 *
 * @package Zephir
 */
final class Application extends BaseApplication
{
    private $logo = '
 _____              __    _
/__  /  ___  ____  / /_  (_)____
  / /  / _ \/ __ \/ __ \/ / ___/
 / /__/  __/ /_/ / / / / / /
/____/\___/ .___/_/ /_/_/_/
         /_/
';

    /**
     * The Service Registrator instance.
     *
     * @var ServiceRegistrator
     */
    protected $serviceRegistrator;

    /**
     * Bootstrap constructor.
     *
     * @param string             $basePath  The Zephir compiler base directory.
     * @param ContainerInterface $container The DI container (if any).
     */
    public function __construct($basePath, ContainerInterface $container = null)
    {
        parent::__construct('Zephir', Zephir::VERSION);

        try {
            $this->serviceRegistrator = new ServiceRegistrator($basePath, $container);
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
     * Register Zephir compiler.
     *
     * @return void
     */
    protected function registerCompiler()
    {
        $this->serviceRegistrator->registerService(
            new CompilerProvider()
        );
    }

    /**
     * {@inheritdoc}
     *
     * @param  Command $command
     * @return Command|null
     */
    public function add(Command $command)
    {
        if ($command instanceof ContainerAwareCommand) {
            $command->setContainer($this->serviceRegistrator->getContainer());
        }

        return parent::add($command);
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getHelp()
    {
        return $this->logo . PHP_EOL . parent::getHelp();
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
     * @param  InputInterface $input
     * @param  OutputInterface $output
     * @return int
     *
     * @throws \Throwable|\Exception
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
     * Formats error message to show an exception opening the file and highlighting the wrong part.
     *
     * @todo
     * @deprecated
     *
     * @param  \Exception $exception
     * @param  Config     $config Current config object [optional].
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

        $container = $this->serviceRegistrator->getContainer();

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
