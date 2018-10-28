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

use RecursiveDirectoryIterator;
use RecursiveIteratorIterator;
use ReflectionClass;
use SplObjectStorage;
use Zephir\Di\ContainerAwareTrait;
use Zephir\Di\InjectionAwareInterface;
use Zephir\Environment;
use Zephir\Exception\BadMethodCallException;
use Zephir\Exception\OutOfBoundsException;
use Zephir\Exception\ValidationException;

/**
 * Zephir\Commands\Manager
 *
 * @package Zephir\Commands
 */
class Manager extends SplObjectStorage implements InjectionAwareInterface
{
    use ContainerAwareTrait {
        ContainerAwareTrait::__construct as protected __DiInject;
    }

    private $similarSounds = [];

    /**
     * Registers builtin commands.
     *
     * @return void
     *
     * @throws \ReflectionException
     */
    public function registerBuiltinCommands()
    {
        /** @var Environment $environment */
        $environment = $this->container->get(Environment::class);
        $environment->getPath();

        $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator(
            $this->container->get(Environment::class)->getPath('Library/Commands')
        ));

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

            $command = $command->newInstanceArgs([$this, $this->container]);

            /** @var CommandAbstract $command */
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
     * @param  string $action  Action name.
     * @return CommandInterface
     *
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
