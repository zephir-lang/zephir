<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

/**
 * AliasManager
 *
 * Manage aliases in a file
 */
class AliasManager
{

    protected $aliases = array();

    /**
     * Adds a renaming in a "use" to the alias manager
     *
     * @param array $useStatement
     */
    public function add(array $useStatement)
    {
        foreach ($useStatement['aliases'] as $alias) {
            if (isset($alias['alias'])) {
                $this->aliases[$alias['alias']] = $alias['name'];
            } else {
                $parts = explode("\\", $alias['name']);
                $implicitAlias = $parts[count($parts) - 1];
                $this->aliases[$implicitAlias] = $alias['name'];
            }
        }
    }

    /**
     * Checks if a class name is an existing alias
     *
     * @param string $alias
     * @return boolean
     */
    public function isAlias($alias)
    {
        return isset($this->aliases[$alias]);
    }

    /**
     * Returns the class name according to an existing alias
     *
     * @param string $alias
     * @return string
     */
    public function getAlias($alias)
    {
        return $this->aliases[$alias];
    }
}
