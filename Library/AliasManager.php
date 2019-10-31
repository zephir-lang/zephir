<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
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
    public function getAlias(string $alias): bool
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
     * Check if Namespace use an Aliasing in use statement.
     *
     * ex: use Events\ManagerInterface as EventsManagerInterface;
     *
     * @param string $alias
     *
     * @return bool
     */
    public function hasUseStatementAliased(string $alias): bool
    {
        if ($this->isAlias($alias)) {
            return $alias !== $this->implicitAlias($this->getAlias($alias));
        }

        return false;
    }

    /**
     * Extract implicit alias from use statement.
     *
     * @param string $namespace - FQN or simple Class name from use statement
     *
     * @return string
     */
    private function implicitAlias(string $namespace): string
    {
        $parts = explode('\\', $namespace);

        return $parts[\count($parts) - 1];
    }
}
