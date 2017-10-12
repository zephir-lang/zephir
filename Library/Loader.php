<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
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
