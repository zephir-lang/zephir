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

use ReflectionClass;
use SplObjectStorage;
use RecursiveIteratorIterator;
use RecursiveDirectoryIterator;
use Zephir\CommandArgumentParser;
use Zephir\Exception\ValidationException;
use Zephir\Exception\OutOfBoundsException;
use Zephir\Exception\BadMethodCallException;

/**
 * Zephir\Commands\Manager
 *
 * @package Zephir\Commands
 */
class Manager extends SplObjectStorage
{
    private $similarSounds = [];

    /**
     * Command argument parser
     *
     * @var CommandArgumentParser
     */
    private $argumentParser;

    /**
     * Manager constructor.
     */
    public function __construct()
    {
        $this->argumentParser = new CommandArgumentParser();
    }

    /**
     * Registers builtin commands.
     *
     * @return void
     */
    public function registerBuiltinCommands()
    {
        $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator(dirname(__FILE__)));
        $iterator->rewind();

        while ($iterator->valid()) {
            $fileInfo = $iterator->current();
            if ($fileInfo->isDir() || $fileInfo->getExtension() !== 'php') {
                $iterator->next();
                continue;
            }

            $className = __NAMESPACE__ . '\\' . $fileInfo->getBasename('.php');
            if ($className === __CLASS__) {
                $iterator->next();
                continue;
            }

            $command = new ReflectionClass($className);
            if ($command->isAbstract() || $command->isInterface()) {
                $iterator->next();
                continue;
            }

            if (!$command->implementsInterface(CommandInterface::class)) {
                $iterator->next();
                continue;
            }

            $command = $command->newInstanceArgs([$this]);
            $data = [
                'usage'       => $command->getUsage(),
                'description' => $command->getDescription(),
                'synopsis'    => $command->getSynopsis(),
            ];

            $this->attach($command, (object) $data);
            $iterator->next();
        }
    }

    /**
     * {@inheritdoc}
     *
     * @param CommandInterface $object object whose identifier is to be calculated.
     * @return string
     * @throws ValidationException
     */
    public function getHash($object)
    {
        $this->validate($object);

        return $object->getCommand();
    }

    /**
     * Gets Command argument parser
     *
     * @return CommandArgumentParser
     */
    public function getCommandArgumentParser()
    {
        return $this->argumentParser;
    }

    /**
     * {@inheritdoc}
     *
     * @param CommandInterface|object $object The command to add.
     * @param mixed $data
     * @throws ValidationException
     */
    public function attach($object, $data = null)
    {
        $this->validate($object);

        parent::attach($object, $data);

        $action = $this->getHash($object);
        $this->similarSounds[metaphone($action)] = $action;
    }

    /**
     * {@inheritdoc}
     *
     * @param CommandInterface|object $object The command to remove.
     * @return void
     * @throws ValidationException
     */
    public function detach($object)
    {
        $this->validate($object);

        parent::detach($object);

        $action = $this->getHash($object);
        unset($this->similarSounds[metaphone($action)]);
    }

    /**
     * Resolves and returns a compiller command.
     *
     * @param string $action Action name
     * @return CommandInterface
     * @throws OutOfBoundsException
     * @throws BadMethodCallException
     */
    public function resolveByActionName($action)
    {
        if (!$this->count()) {
            throw new OutOfBoundsException(
                'The command stack is not initialized yet. No one command available at this time.'
            );
        }

        if (!is_string($action) || empty($action)) {
            throw new BadMethodCallException(
                'A command name must be a nonempty string.'
            );
        }

        // Sanitize
        $action = trim($action);

        $this->rewind();
        while ($this->valid()) {
            /** @var CommandInterface $command */
            $command = $this->current();
            if ($command->getCommand() === $action) {
                return $command;
            }

            $this->next();
        }

        $message = sprintf("Unrecognized action '%s'.", $action);
        $metaphone = metaphone($action);

        foreach ($this->similarSounds as $alias => $name) {
            if ($alias == $metaphone) {
                $message .= sprintf(" Did you mean '%s'?", $name);
            }
        }

        throw new OutOfBoundsException($message);
    }

    /**
     * Internal validator.
     *
     * @param mixed $object
     * @throws ValidationException
     */
    private function validate($object)
    {
        if (!is_object($object) || !$object instanceof CommandInterface) {
            if (is_object($object)) {
                $got = get_class($object);
            } else {
                $got = gettype($object);
            }

            throw new ValidationException(
                sprintf('Command must implement %s got %s.', CommandInterface::class, $got)
            );
        }
    }
}
