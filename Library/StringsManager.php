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

/**
 * Class StringsManager.
 *
 * Manages the concatenation keys for the extension and the interned strings
 */
abstract class StringsManager
{
    /**
     * List of headers.
     *
     * @var array
     */
    protected $concatKeys = [
        'vv' => true,
        'vs' => true,
        'sv' => true,
    ];

    /**
     * Adds a concatenation combination to the manager.
     *
     * @param string $key
     */
    public function addConcatKey($key)
    {
        $this->concatKeys[$key] = true;
    }

    /**
     * Generates the concatenation code.
     *
     * @return array
     */
    abstract public function genConcatCode();

    /**
     * Obtains the existing concatenation keys.
     *
     * @return array
     */
    public function getConcatKeys()
    {
        return $this->concatKeys;
    }
}
