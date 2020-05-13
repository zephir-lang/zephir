<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * LICENSE the file that was distributed with this source code.
 */

use Stub\Properties\ProtectedProperties;

class ExtendsProtectedProperties extends ProtectedProperties
{
    protected $someArrayVar = [
        'key' => 'value',
    ];
}
