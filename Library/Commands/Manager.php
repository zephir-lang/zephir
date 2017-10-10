<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir (https://zephir-lang.com/)             |
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
use Zephir\Exception\InvalidArgumentException;

/**
 * Zephir\Commands\Manager
 *
 * @package Zephir\Commands
 */
class Manager extends SplObjectStorage
{
    /**
     * Registers builtin commands.
     *
     * @return void
     */
    public function registerBuiltinCommands()
    {
        $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator(dirname(__FILE__)));
        $iterator->rewind();

        while($iterator->valid()) {
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

            $this->attach($command->newInstance());
            $iterator->next();
        }
    }

    /**
     * Calculate a unique identifier for the contained command.
     *
     * @param CommandInterface $object object whose identifier is to be calculated.
     * @return string
     * @throws InvalidArgumentException
     */
    public function getHash($object)
    {
        if (!is_object($object) || $object instanceof CommandInterface) {
            throw new InvalidArgumentException('Command must implement ' . CommandInterface::class);
        }

        return $object->getCommand();
    }
}
