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

use Exception;

class ConfigException extends Exception
{
    public function __construct($message = '', $code = 0, Exception $previous = null)
    {
        $message .= PHP_EOL.'Please see http://zephir-lang.com/config.html for more information';

        parent::__construct($message, $code, $previous);
    }
}
