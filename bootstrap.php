<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

error_reporting(-1);

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);

setlocale(LC_ALL, 'en_US.utf-8');

set_time_limit(-1);

if (!ini_get('date.timezone')) {
    ini_set('date.timezone', 'UTC');
}

clearstatcache();

define('ZEPHIRPATH', __DIR__ . DIRECTORY_SEPARATOR);

define('T', "\t");
define('2T', "\t\t");

if (file_exists(__DIR__ . '/vendor/autoload.php')) {
    require __DIR__ . '/vendor/autoload.php';
} else {
    require __DIR__ . '/Library/Loader.php';
    Zephir\Loader::register();
}
