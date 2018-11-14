<?php

/**
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
use Zephir\Command\ContainerAwareCommand;
use Zephir\Commands\Manager;
use Zephir\Exception\ExceptionInterface;
use Zephir\Providers\CompilerProvider;

/**
 * Zephir\Bootstrap
 *
 * @package Zephir
 */
final class Application extends BaseApplication
{
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
        $this->serviceRegistrator = new ServiceRegistrator($basePath, $container);
        $this->registerCompiler();

        $container = $this->serviceRegistrator->getContainer();

        parent::__construct('Zephir', (string) $container->get(Version::class));
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
     * Boots the compiler executing the specified action.
     *
     * @todo
     * @deprecated
     * @return void
     */
    public function execute()
    {
        $container = $this->serviceRegistrator->getContainer();

        if (isset($_SERVER['argv'][1])) {
            $action = $_SERVER['argv'][1];
        } else {
            $action = 'help';
        }

        try {
            /** @var Manager $manager */
            $manager = $container->get(Manager::class);
            $command = $manager->resolveByActionName($action);

            // Execute the command
            $command->execute($container->get(Config::class), $container->get(Logger::class));
        } catch (\Exception $e) {
            fwrite(
                STDERR,
                $this->formatErrorMessage($e, $container->has(Config::class) ? $container->get(Config::class) : null)
            );

            exit(1);
        }
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
        $environment = $container->has(Environment::class) ? $container->get(Environment::class) : null;

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
