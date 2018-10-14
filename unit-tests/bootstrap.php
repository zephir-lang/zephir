<?php

/*
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

define('TESTS_PATH', dirname(__FILE__));
define('DATA_PATH', dirname(__FILE__) . '/Data');

defined('ZEPHIRPATH') || define('ZEPHIRPATH', dirname(__DIR__));

if (!extension_loaded('phalcon')) {
    include_once ZEPHIRPATH . '/prototypes/phalcon.php';
}

if (!extension_loaded('test')) {
    if (ini_get('enable_dl') == '1') {
        $prefix = (PHP_SHLIB_SUFFIX === 'dll') ? 'php_' : '';
        dl($prefix . 'test.' . PHP_SHLIB_SUFFIX);
    } else {
        exit('"test" extension not loaded; cannot run tests without it');
    }
}
