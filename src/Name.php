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

final class Name
{
    public static function camelize(string $string): string
    {
        return str_replace(' ', '', ucwords(str_replace('_', ' ', $string)));
    }

    /**
     * Transform class/interface name to
     * Fully qualified path name (FQN) format.
     */
    public static function fetchFQN(
        string $class,
        ?string $namespace = null,
        ?AliasManager $aliasManager = null
    ): string {
        /**
         * Absolute class/interface name
         */
        if ('\\' === $class[0]) {
            return substr($class, 1);
        }

        /**
         * If class/interface name not begin with \
         * maybe an alias or a sub-namespace
         */
        $firstSepPos = strpos($class, '\\');
        if (false !== $firstSepPos) {
            $baseName = substr($class, 0, $firstSepPos);
            if ($aliasManager && $aliasManager->isAlias($baseName)) {
                return $aliasManager->getAlias($baseName).'\\'.substr($class, $firstSepPos + 1);
            }
        } elseif ($aliasManager && $aliasManager->isAlias($class)) {
            return $aliasManager->getAlias($class);
        }

        return $namespace ? $namespace.'\\'.$class : $class;
    }
}
