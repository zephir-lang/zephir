<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use const PHP_BINARY;
use const PHP_EOL;
use const PHP_SAPI;
use const PHP_VERSION;
use const STDERR;

if (version_compare('7.0.0', PHP_VERSION, '>')) {
    fprintf(
        STDERR,
        'This Zephir version is supported on PHP >= 7.0.0.'.PHP_EOL.
        'You are using PHP %s (%s).'.PHP_EOL,
        PHP_VERSION,
        PHP_BINARY
    );

    exit(1);
}

if (PHP_SAPI !== 'cli' && PHP_SAPI !== 'phpdbg') {
    fprintf(
        STDERR,
        'Zephir should be invoked via the CLI version of PHP, not the %s SAPI.'.PHP_EOL,
        PHP_SAPI
    );

    exit(1);
}

$autoloaders = [
    __DIR__.'/../vendor/autoload.php', // Is installed locally
    __DIR__.'/../../../autoload.php',  // Is installed via Composer
];

foreach ($autoloaders as $file) {
    if (file_exists($file)) {
        include_once $file;
        break;
    }
}

if (false == class_exists('Composer\Autoload\ClassLoader', false)) {
    fwrite(STDERR, 'Unable to find the Composer autoloader.'.PHP_EOL);

    exit(1);
}
