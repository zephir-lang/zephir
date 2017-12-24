<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */

namespace Zephir;

use Zephir\Commands\Manager;
use Zephir\Exception\ExceptionInterface;

/**
 * Zephir\Bootstrap
 *
 * Main compiler bootstrap
 *
 * @package Zephir
 */
class Bootstrap
{
    /**
     * The Zephir base direcrory.
     * @var string
     */
    private $baseDir;

    /**
     * Commands manager.
     * @var Manager
     */
    private $commandsManager;

    /**
     * Bootstrap constructor.
     *
     * @param string $baseDir The base Zephir direcrory.
     */
    public function __construct($baseDir = null)
    {
        $baseDir = realpath($baseDir ?: dirname(__DIR__));

        if (!is_string($baseDir) || !is_dir($baseDir)) {
            fwrite(
                STDERR,
                sprintf(
                    "Unable to locate Zephir installation path.\n\nDouble check Zephir installation " .
                    "and/or try to setup ZEPHIRDIR variable to the proper Zephir installation path.\n\n" .
                    "Current ZEPHIRDIR value: %s\nThe base path passed to the Bootstrap: %s\n",
                    getenv('ZEPHIRDIR'),
                    is_string($baseDir) ? $baseDir : gettype($baseDir)
                )
            );

            exit(1);
        }

        $this->baseDir = rtrim($baseDir, '\\/');
        $this->commandsManager = new Manager();
    }

    /**
     * Gets the Zephir base direcrory.
     *
     * @return string
     */
    public function getBaseDir()
    {
        return $this->baseDir;
    }

    /**
     * Gets currently initialized Command Manager.
     *
     * @return Manager
     */
    public function getCommandsManager()
    {
        return $this->commandsManager;
    }

    /**
     * Boots the compiler executing the specified action.
     *
     * @return void
     */
    public function boot()
    {
        try {
            // Global Config
            $config = Config::fromServer();

            // Global Logger
            $logger = new Logger($config);

            if (isset($_SERVER['argv'][1])) {
                $action = $_SERVER['argv'][1];
            } else {
                $action = 'help';
            }

            $manager = new Manager();
            $manager->registerBuiltinCommands();

            $command = $manager->resolveByActionName($action);

            // Execute the command
            $command->execute($config, $logger);
        } catch (\Exception $e) {
            fwrite(STDERR, $this->formatErrorMessage($e, isset($config) ? $config : null));
            exit(1);
        }
    }

    /**
     * Formats error message to show an exception opening the file and highlighting the wrong part.
     *
     * @param \Exception $e
     * @param Config $config Current config object [optional].
     * @return string
     */
    protected function formatErrorMessage(\Exception $e, Config $config = null)
    {
        $message = '';

        if ($config && $config->get('verbose')) {
            $message .= sprintf('[%s]: ', get_class($e));
        }

        $message .= $e->getMessage() . PHP_EOL;

        if ($e instanceof ExceptionInterface && $extraInfo = $e->getErrorRegion()) {
            $message .= sprintf("\n%s", $extraInfo);
        }

        $message .= PHP_EOL;
        if (!$config || !$config->get('verbose')) {
            return $message;
        }

        $base = $this->baseDir;
        $preparePaths = function ($path) use ($base) {
            if (is_string($base)) {
                $path = str_replace(rtrim($base, '\\/') . DIRECTORY_SEPARATOR, '', $path);
            }

            return $path;
        };

        $message .= sprintf("at %s(%s)\n\n", $preparePaths($e->getFile()), $e->getLine());
        $message .= sprintf("Stack trace:\n%s\n", $preparePaths($e->getTraceAsString()));

        return $message;
    }
}
