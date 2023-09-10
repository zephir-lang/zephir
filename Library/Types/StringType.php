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

namespace Zephir\Types;

/**
 * Encapsulates built-in methods for the "string" type
 *
 * TODO: explode, join, replace
 */
class StringType extends AbstractType
{
    public array $methodMap = [
        'index' => 'strpos',
        'trim' => 'trim',
        'trimleft' => 'ltrim',
        'trimright' => 'rtrim',
        'length' => 'strlen',
        'lower' => 'strtolower',
        'upper' => 'strtoupper',
        'lowerfirst' => 'lcfirst',
        'upperfirst' => 'ucfirst',
        'format' => 'sprintf',
        'md5' => 'md5',
        'sha1' => 'sha1',
        'nl2br' => 'nl2br',
        'parsecsv' => 'str_getcsv',
        'parsejson' => 'json_decode',
        'tojson' => 'json_encode',
        'repeat' => 'str_repeat',
        'shuffle' => 'str_shuffle',
        'split' => 'str_split',
        'compare' => 'strcmp',
        'comparelocale' => 'strcoll',
        'rev' => 'strrev',
        'htmlspecialchars' => 'htmlspecialchars',
        'camelize' => 'camelize',
        'uncamelize' => 'uncamelize',
    ];

    public function getTypeName(): string
    {
        return Types::T_STRING;
    }
}
