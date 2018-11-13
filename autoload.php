<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

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

$autoloaders = [
    __DIR__ . '/vendor/autoload.php',
    __DIR__ . '/../../autoload.php',
];

foreach ($autoloaders as $file) {
    if (file_exists($file)) {
        define('ZEPHIR_COMPOSER_INSTALL', $file);
        break;
    }
}

unset($file);

if (!defined('ZEPHIR_COMPOSER_INSTALL')) {
    fwrite(
        STDERR,
        'You need to install the Zephir dependencies using Composer:' . PHP_EOL . PHP_EOL .
        '    composer install' . PHP_EOL . PHP_EOL .
        'You can learn all about Composer on https://getcomposer.org/.' . PHP_EOL
    );

    exit(1);
}

require ZEPHIR_COMPOSER_INSTALL;
