<?php

/**
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */


namespace Zephir;

/**
 * Class Loader
 * Loads classes when the composer autoloader is not installed
 *
 * @package Zephir
 */
class Loader
{
    /**
     * Register autoload
     */
    public static function register()
    {
        spl_autoload_register(array(__CLASS__, 'autoload'));
    }

    /**
     * @param string $className
     */
    public static function autoload($className)
    {
        $filename = __DIR__ .
            str_replace(
                'Zephir' . DIRECTORY_SEPARATOR,
                DIRECTORY_SEPARATOR,
                str_replace('\\', DIRECTORY_SEPARATOR, $className)
            ) . '.php';

        if (file_exists($filename)) {
            require $filename;
        }
    }
}
