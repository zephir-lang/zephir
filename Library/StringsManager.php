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

namespace Zephir;

/**
 * Class StringsManager
 *
 * Manages the concatenation keys for the extension and the interned strings
 */
abstract class StringsManager
{
    /**
     * List of headers
     * @var array
     */
    protected $concatKeys = array(
        'vv' => true,
        'vs' => true,
        'sv' => true
    );

    /**
     * Adds a concatenation combination to the manager
     *
     * @param string $key
     */
    public function addConcatKey($key)
    {
        $this->concatKeys[$key] = true;
    }

    /**
     * Generates the concatenation code
     *
     * @return array
     */
    abstract public function genConcatCode();

    /**
     * Obtains the existing concatenation keys
     *
     * @return array
     */
    public function getConcatKeys()
    {
        return $this->concatKeys;
    }
}
