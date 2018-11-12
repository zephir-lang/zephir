<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

use Composer\Autoload\ClassLoader;

$autoloadFile = './vendor/phalcon/zephir/autoload.php';

if (file_exists('./vendor/autoload.php') && file_exists($autoloadFile) && __FILE__ != realpath($autoloadFile)) {
    // The global installation or phar file
    require $autoloadFile;

    // Require 'package/bin' instead of 'zephir' to avoid printing hashbang line
    require './vendor/phalcon/zephir/package/bin';

    die;
} elseif (file_exists(__DIR__ . '/vendor/autoload.php')) {
    // PHAR
    require_once(__DIR__ . '/vendor/autoload.php');
} elseif (file_exists(__DIR__ . '/../../autoload.php')) {
    // Composer
    require_once __DIR__ . '/../../autoload.php';
}

unset($autoloadFile);

if (version_compare('5.6.0', PHP_VERSION, '>')) {
    fprintf(
        STDERR,
        'This Zephir version is supported on PHP >= 5.6.0.' . PHP_EOL .
        'You are using PHP %s (%s).' . PHP_EOL,
        PHP_VERSION,
        PHP_BINARY
    );

    exit(1);
}

if (class_exists(ClassLoader::class, false) == false) {
    fwrite(
        STDERR,
        'You need to install the Zephir dependencies using Composer:' . PHP_EOL . PHP_EOL .
        '    composer install' . PHP_EOL . PHP_EOL .
        'You can learn all about Composer on https://getcomposer.org/.' . PHP_EOL
    );

    exit(1);
}
