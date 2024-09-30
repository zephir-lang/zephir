<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir;

use function array_flip;
use function array_keys;
use function count;
use function explode;
use function in_array;
use function trim;

/**
 * Manage aliases in a file
 */
final class AliasManager
{
    protected array $aliases = [];

    /**
     * Adds a renaming in a "use" to the alias manager.
     */
    public function add(array $useStatement): void
    {
        foreach ($useStatement['aliases'] as $alias) {
            $implicitAlias = $alias['alias'] ?? $this->implicitAlias($alias['name']);

            $this->aliases[$implicitAlias] = $alias['name'];
        }
    }

    /**
     * Returns the class name according to an existing alias.
     */
    public function getAlias(string $alias): string
    {
        return $this->aliases[$alias];
    }

    /**
     * Returns alias by fully qualified class name.
     *
     * @param string $className - fully qualified class name
     */
    public function getAliasForClassName(string $className): string
    {
        $keys = array_keys($this->aliases, trim($className, '\\'));

        if (1 === count($keys)) {
            return $keys[0];
        }

        return $className;
    }

    /**
     * Returns key-value pair of aliases.
     */
    public function getAliases(): array
    {
        return $this->aliases;
    }

    /**
     * Checks if a class name is an existing alias.
     */
    public function isAlias(string $alias): bool
    {
        return isset($this->aliases[$alias]);
    }

    /**
     * Check if class name has explicit alias in `use` declaration.
     *
     * @param string $className - fully qualified class name
     */
    public function isAliasPresentFor(string $className): bool
    {
        $extractAlias = $this->implicitAlias($className);

        $isClassDeclared = in_array($className, $this->aliases);
        $classAlias      = array_flip($this->aliases)[$className] ?? null;

        return $isClassDeclared && $classAlias !== $extractAlias;
    }

    /**
     * Check if class name use an aliasing in use statement.
     *
     * ex: use Events\ManagerInterface as EventsManagerInterface;
     */
    public function isUseStatementAliased(string $alias): bool
    {
        if ($this->isAlias($alias)) {
            return $alias !== $this->implicitAlias($this->getAlias($alias));
        }

        return false;
    }

    /**
     * Extract implicit alias from use statement.
     *
     * @param string $className - FQCN or simple class name from use statement
     */
    private function implicitAlias(string $className): string
    {
        $parts = explode('\\', $className);

        return $parts[count($parts) - 1];
    }
}
