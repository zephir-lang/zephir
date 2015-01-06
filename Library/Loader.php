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
        require __DIR__ .
            str_replace(
                'Zephir' . DIRECTORY_SEPARATOR,
                DIRECTORY_SEPARATOR,
                str_replace('\\', DIRECTORY_SEPARATOR, $className)
            ) . '.php';
    }
}
