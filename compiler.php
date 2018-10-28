<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

use Zephir\Bootstrap;

require_once __DIR__ . '/vendor/autoload.php';

$bootstrap = new Bootstrap(
    strpos(__DIR__, 'phar://') === 0 ?
        (getenv('ZEPHIRDIR') ?: __DIR__) :
        __DIR__
);

$bootstrap->execute();
