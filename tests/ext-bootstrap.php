<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

/**
 * Draw message in the Box
 */
function messageBox(string $message): string {
    $line = str_repeat('━', strlen($message) + 2);
    $topline = "┏{$line}┓";
    $bottomline = "┗{$line}┛";

    return sprintf("%s\n┃ %s ┃\n%s\n\n", $topline, $message, $bottomline);
}

if (!extension_loaded('phalcon')) {
    include_once __DIR__.'/../prototypes/phalcon.php';
}

if (!extension_loaded('psr')) {
    $message = messageBox("The 'psr' extension is not loaded; Cannot run tests without it.");
    exit($message);
}

if (!extension_loaded('stub')) {
    if ('1' == ini_get('enable_dl')) {
        $prefix = (PHP_SHLIB_SUFFIX === 'dll') ? 'php_' : '';
        dl($prefix.'stub.'.PHP_SHLIB_SUFFIX);
    } else {
        $message = messageBox("The 'stub' extension is not loaded; Cannot run tests without it.");
        exit($message);
    }
}
