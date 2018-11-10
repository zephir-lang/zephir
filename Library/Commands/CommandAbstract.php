<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
namespace Zephir\Commands;

use Psr\Container\ContainerInterface;
use Zephir\CommandArgumentParser;
use Zephir\Compiler;
use Zephir\Config;
use Zephir\Di\ContainerAwareTrait;
use Zephir\Di\InjectionAwareInterface;
use Zephir\Logger;

/**
 * Zephir\Commands\CommandAbstract
 *
 * Provides a superclass for any command.
 *
 * @package Zephir\Commands
 */
abstract class CommandAbstract implements CommandInterface, InjectionAwareInterface
{
    use ContainerAwareTrait {
        ContainerAwareTrait::__construct as protected __DiInject;
    }

    private $parameters = [];

    /**
     * Currently initialized Command Manager.
     * @var Manager
     */
    private $commandsManager;

    /**
     * The Zephir base direcrory.
     * @var string
     */
    private $baseDir;

    /**
     * CommandAbstract constructor.
     *
     * @param Manager                 $commandsManager
     * @param ContainerInterface|null $container
     */
    public function __construct(Manager $commandsManager, ContainerInterface $container = null)
    {
        $this->__DiInject($container);

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
            /** @var CommandArgumentParser $parser */
            $parser = $this->getContainer()->get(CommandArgumentParser::class);
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

        /** @var CommandArgumentParser $parser */
        $parser = $this->getContainer()->get(CommandArgumentParser::class);

        return $parser->hasHelpOption($params);
    }

    /**
     * {@inheritdoc}
     *
     * @param  Config $config
     * @param  Logger $logger
     * @return void
     */
    public function execute(Config $config, Logger $logger)
    {
        if ($this->hasHelpOption()) {
            echo $this->getSynopsis();
            return;
        }

        $compiler = $this->container->get(Compiler::class);

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
