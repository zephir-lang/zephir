<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

set_error_handler(
    static function ($code, $message, $file = '', $line = -1) {
        if (error_reporting() & $code) {
            throw new \ErrorException($message, 0, $code, (string) $file, $line);
        }
    }
);
