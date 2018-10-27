<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir;

/**
 * Checks if currently running under MS Windows.
 *
 * @return bool
 */
function is_windows()
{
    return 'WIN' === \strtoupper(\substr(PHP_OS, 0, 3));
}

/**
 * Checks if currently running under macOs.
 *
 * @return bool
 */
function is_macos()
{
    return 'DARWIN' === \strtoupper(\substr(PHP_OS, 0, 6));
}

/**
 * Checks if currently running under BSD based OS.
 *
 * @link   https://en.wikipedia.org/wiki/List_of_BSD_operating_systems
 * @return bool
 */
function is_bsd()
{
    return false !== \stristr(\strtolower(PHP_OS), 'bsd');
}

/**
 * Attempts to remove recursively the directory with all subdirectories and files.
 *
 * A E_WARNING level error will be generated on failure.
 *
 * @param  string $path
 * @return void
 */
function unlink_recursive($path)
{
    if (\is_dir($path)) {
        $objects = \array_diff(\scandir($path), ['.', '..']);

        foreach ($objects as $object) {
            if (\is_dir("{$path}/{$object}")) {
                unlink_recursive("{$path}/{$object}");
            } else {
                \unlink("{$path}/{$object}");
            }
        }

        \rmdir($path);
    }
}

/**
 * Camelize a string.
 *
 * @param  string $string
 * @return string
 */
function camelize($string)
{
    return str_replace(' ', '', ucwords(str_replace('_', ' ', $string)));
}


/**
 * Prepares a class name to be used as a C-string.
 *
 * @param  string $className
 * @return string
 */
function escape_class($className)
{
    return str_replace('\\', '\\\\', $className);
}
