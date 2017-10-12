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

namespace Zephir\Commands;

use Zephir\Config;
use Zephir\Logger;
use Zephir\Parser;
use Zephir\Compiler;
use Zephir\BaseBackend;
use Zephir\Exception\IllegalStateException;

/**
 * CommandAbstract
 *
 * Provides a superclass for any command.
 *
 * @package Zephir\Commands
 */
abstract class CommandAbstract implements CommandInterface
{
    private $parameters = [];

    /**
     * Currently initialized Command Manager.
     * @var Manager
     */
    private $commandsManager;

    /**
     * CommandAbstract constructor.
     *
     * @param Manager $commandsManager
     */
    public function __construct(Manager $commandsManager)
    {
        $this->commandsManager = $commandsManager;
    }

    /**
     * {@inheritdoc}
     *
     * @return Manager
     */
    public function getCommandsManager()
    {
        return $this->commandsManager;
    }

    /**
     * Sets named parameter.
     *
     * @param string $name
     * @param string $value
     * @return void
     */
    protected function setParameter($name, $value)
    {
        $this->parameters[$name] = $value;
    }

    /**
     * Gets named parameter.
     *
     * Returns parameter named $name if specified on the command line otherwise returns NULL.
     *
     * @param string $name
     * @return string|null
     */
    public function getParameter($name)
    {
        return isset($this->parameters[$name]) ? $this->parameters[$name] : null;
    }

    /**
     * Parse the input arguments for the command and returns theme as an associative array.
     *
     * @return array the list of the parameters
     */
    public function parseArguments()
    {
        $params = [];

        if (count($_SERVER['argv']) > 2) {
            $commandArgs = array_slice($_SERVER['argv'], 2);
            $parser = $this->getCommandsManager()->getCommandArgumentParser();
            $params = $parser->parseArgs(array_merge(['command'], $commandArgs));
        }

        return $params;
    }

    /**
     * Whether the current command called with help option.
     *
     * @return bool
     */
    public function hasHelpOption()
    {
        $params = $this->parseArguments();
        $parser = $this->getCommandsManager()->getCommandArgumentParser();

        return $parser->hasHelpOption($params);
    }

    /**
     * {@inheritdoc}
     *
     * @param Config $config
     * @param Logger $logger
     */
    public function execute(Config $config, Logger $logger)
    {
        $params = $this->parseArguments();

        if ($this->hasHelpOption()) {
            echo $this->getSynopsis();
            return;
        }

        $backend = empty($params['backend']) ? BaseBackend::getActiveBackend() : $params['backend'];
        $className = "Zephir\\Backends\\{$backend}\\Backend";

        if (!class_exists($className)) {
            throw new IllegalStateException("Backend {$backend} doesn't exist.");
        }

        $backend = new $className($config);

        $parserManager = new Parser\Manager(new Parser(), $logger, $params);
        $compiler = new Compiler($config, $logger, $backend, $parserManager);

        $command = $this->getCommand();
        $compiler->$command($this);
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getSynopsis()
    {
        $template =<<<EOF

Name:
    %s -- %s

Synopsis:
    zephir %s


EOF;
        return sprintf(
            $template,
            $this->getCommand(),
            rtrim($this->getDescription(), '.') . '.',
            $this->getUsage()
        );
    }
}
