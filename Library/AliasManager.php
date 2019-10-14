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
            if (isset($alias['alias'])) {
                $this->aliases[$alias['alias']] = $alias['name'];
            } else {
                $parts = explode('\\', $alias['name']);
                $implicitAlias = $parts[\count($parts) - 1];
                $this->aliases[$implicitAlias] = $alias['name'];
            }
        }
    }

    /**
     * Checks if a class name is an existing alias.
     *
     * @param string $alias
     *
     * @return bool
     */
    public function isAlias($alias)
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
    public function getAlias($alias)
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
}
