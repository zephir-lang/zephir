<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

require __DIR__ . '/bootstrap.php';

$bootstrap = new Zephir\Bootstrap(getenv('ZEPHIRDIR') ?: __DIR__);
$bootstrap->boot();
