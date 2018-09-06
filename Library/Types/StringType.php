<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Types;

/**
 * StringType
 *
 * Encapsulates built-in methods for the "string" type
 * @TODO explode, join, replace
 */
class StringType extends AbstractType
{
    /**
     * {@inheritdoc}
     */
    public $methodMap = array(
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
        'toutf8' => 'utf8_encode',
        'repeat' => 'str_repeat',
        'shuffle' => 'str_shuffle',
        'split' => 'str_split',
        'compare' => 'strcmp',
        'comparelocale' => 'strcoll',
        'rev' => 'strrev',
        'htmlspecialchars' => 'htmlspecialchars',
        'camelize' => 'camelize',
        'uncamelize' => 'uncamelize',
    );

    /**
     * {@inheritdoc}
     */
    public function getTypeName()
    {
        return 'string';
    }
}
