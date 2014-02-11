<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

use Zephir\Commands\CommandAbstract;

require ZEPHIRPATH . 'Library/Commands/Interface.php';
require ZEPHIRPATH . 'Library/Commands/Abstract.php';

/**
 * Bootstrap
 *
 * Main compiler bootstrap
 */
class Bootstrap
{
    /**
     * @var array|CommandAbstract[]
     */
    protected static $_commands = array();

    /**
     * Shows an exception opening the file and highlighing the wrong part
     *
     * @param Exception $e
     * @param Config $config
     */
    protected static function showException(\Exception $e, Config $config = null)
    {
        echo get_class($e), ': ', $e->getMessage(), PHP_EOL;
        if (method_exists($e, 'getExtra')) {
            $extra = $e->getExtra();
            if (is_array($extra)) {
                if (isset($extra['file'])) {
                    echo PHP_EOL;
                    $lines = file($extra['file']);
                    if (isset($lines[$extra['line'] - 1])) {
                        $line = $lines[$extra['line'] - 1];
                        echo "\t", str_replace("\t", " ", $line);
                        if (($extra['char'] - 1) > 0) {
                            echo "\t", str_repeat("-", $extra['char'] - 1), "^", PHP_EOL;
                        }
                    }
                }
            }
        }
        echo PHP_EOL;
        if ($config && $config->get('verbose')) {
            echo 'at ', str_replace(ZEPHIRPATH, '', $e->getFile()), '(', $e->getLine(), ')', PHP_EOL;
            echo str_replace(ZEPHIRPATH, '', $e->getTraceAsString()), PHP_EOL;
        }
        exit(1);
    }


    /**
     * Returns the commands registered in the compiler
     *
     * @return array
     */
    public static function getCommands()
    {
        return self::$_commands;
    }

    /**
     * Boots the compiler executing the specified action
     */
    public static function boot()
    {
        try {

            /**
             * Global config
             */
            $config = new Config();

            /**
             * Global logger
             */
            $logger = new Logger($config);

            if (isset($_SERVER['argv'][1])) {
                $action = $_SERVER['argv'][1];
            } else {
                $action = 'help';
            }

            /**
             * Change configurations flags
             */
            if ($_SERVER['argc'] >= 2) {
                for ($i = 2; $i < $_SERVER['argc']; $i++) {

                    $parameter = $_SERVER['argv'][$i];
                    if (preg_match('/^-fno-([a-z0-9\-]+)/', $parameter, $matches)) {
                        $config->set($matches[1], false);
                        continue;
                    }

                    if (preg_match('/^-W([a-z0-9\-]+)/', $parameter, $matches)) {
                        $logger->set($matches[1], false);
                        continue;
                    }

                    switch ($parameter) {

                        case '-w':
                            $config->set('silent', true);
                            break;

                        case '-v':
                            $config->set('verbose', true);
                            break;

                        case '-V':
                            $config->set('verbose', false);
                            break;

                        default:
                            break;
                    }
                }
            }

            /**
             * Register built-in commands
             * @var $item DirectoryIterator
             */
            foreach (new \DirectoryIterator(ZEPHIRPATH . 'Library/Commands') as $item) {
                if (!$item->isDir()) {
                    require_once $item->getRealPath();

                    $className = 'Zephir\Commands\Command' . str_replace('.php', '', $item->getBaseName());
                    $class = new \ReflectionClass($className);

                    if (!$class->isAbstract() && !$class->isInterface()) {
                        /**
                         * @var $command CommandAbstract
                         */
                        $command = new $className();

                        if (!($command instanceof CommandAbstract)) {
                            throw new \Exception('Class ' . $class->name . ' must be instance of CommandAbstract');
                        }

                        self::$_commands[$command->getCommand()] = $command;
                    }
                }
            }

            if (!isset(self::$_commands[$action])) {
                $message = 'Unrecognized action "' . $action . '"';
                $metaphone = metaphone($action);
                foreach (self::$_commands as $key => $command) {
                    if (metaphone($key) == $metaphone) {
                        $message .= PHP_EOL . PHP_EOL . 'Did you mean "' . $key . '"?';
                    }
                }
                throw new \Exception($message);
            }

            /**
             * Execute the command
             */
            self::$_commands[$action]->execute($config, $logger);

        } catch (\Exception $e) {
            self::showException($e, isset($config) ? $config : null);
        }
    }
}
