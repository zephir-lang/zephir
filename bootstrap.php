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

if (file_exists(ZEPHIRPATH . 'vendor/autoload.php')) {
    require ZEPHIRPATH . 'vendor/autoload.php';
} else {
    require ZEPHIRPATH . 'Library/Loader.php';
    Zephir\Loader::register();
}
