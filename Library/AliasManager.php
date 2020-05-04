<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

/**
 * AliasManager.
 *
 * Manage aliases in a file
 */
final class AliasManager
{
    protected $aliases = [];

    /**
     * Adds a renaming in a "use" to the alias manager.
     *
     * @param array $useStatement
     */
    public function add(array $useStatement)
    {
        foreach ($useStatement['aliases'] as $alias) {
            $implicitAlias = $alias['alias'] ?? $this->implicitAlias($alias['name']);

            $this->aliases[$implicitAlias] = $alias['name'];
        }
    }

    /**
     * Checks if a class name is an existing alias.
     *
     * @param string $alias
     *
     * @return bool
     */
    public function isAlias(string $alias): bool
    {
        return isset($this->aliases[$alias]);
    }

    /**
     * Returns the class name according to an existing alias.
     *
     * @param string $alias
     *
     * @return string
     */
    public function getAlias(string $alias): string
    {
        return $this->aliases[$alias];
    }

    /**
     * Returns key-value pair of aliases.
     *
     * @return array
     */
    public function getAliases(): array
    {
        return $this->aliases;
    }

    /**
     * Returns alias by fully qualified class name.
     *
     * @param string $className - fully qualified class name
     *
     * @return string
     */
    public function getAliasForClassName(string $className): string
    {
        $keys = array_keys($this->aliases, trim($className, '\\'));

        if (1 === \count($keys)) {
            return $keys[0];
        }

        return $className;
    }

    /**
     * Check if class name use an aliasing in use statement.
     *
     * ex: use Events\ManagerInterface as EventsManagerInterface;
     *
     * @param string $alias
     *
     * @return bool
     */
    public function isUseStatementAliased(string $alias): bool
    {
        if ($this->isAlias($alias)) {
            return $alias !== $this->implicitAlias($this->getAlias($alias));
        }

        return false;
    }

    /**
     * Check if class name has explicit alias in `use` declaration.
     *
     * @param string $className - fully qualified class name
     *
     * @return bool
     */
    public function isAliasPresentFor(string $className): bool
    {
        $extractAlias = $this->implicitAlias($className);

        $isClassDeclarated = \in_array($className, $this->aliases);
        $classAlias = array_flip($this->aliases)[$className] ?? null;

        return $isClassDeclarated && $classAlias !== $extractAlias;
    }

    /**
     * Extract implicit alias from use statement.
     *
     * @param string $className - FQCN or simple class name from use statement
     *
     * @return string
     */
    private function implicitAlias(string $className): string
    {
        $parts = explode('\\', $className);

        return $parts[\count($parts) - 1];
    }
}
