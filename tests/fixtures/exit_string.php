<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
use Test\ExitDie;

if (!extension_loaded('test')) {
    if ('1' == ini_get('enable_dl')) {
        $prefix = (PHP_SHLIB_SUFFIX === 'dll') ? 'php_' : '';
        dl($prefix.'test.'.PHP_SHLIB_SUFFIX);
    }
}

if (!extension_loaded('test')) {
    exit('"test" extension not loaded; cannot run tests without it');
}

$argv = $_SERVER['argv'];
if (isset($argv[1])) {
    $t = new ExitDie();
    $t->testExit($argv[1]);
}
