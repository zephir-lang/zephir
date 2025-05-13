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

namespace Zephir;

use const PHP_EOL;

class ConfigException extends \Exception
{
    public function __construct($message = '', $code = 0, ?\Exception $previous = null)
    {
        $message .= PHP_EOL . 'Please see https://docs.zephir-lang.com/0.12/en/config for more information';

        parent::__construct($message, $code, $previous);
    }
}
