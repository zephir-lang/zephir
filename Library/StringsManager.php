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

/**
 * Manages the concatenation keys for the extension and the interned strings
 */
abstract class StringsManager
{
    /**
     * List of headers.
     *
     * @var array
     */
    protected array $concatKeys = [
        'vv' => true,
        'vs' => true,
        'sv' => true,
    ];

    /**
     * Adds a concatenation combination to the manager.
     *
     * @param string $key
     */
    public function addConcatKey(string $key)
    {
        $this->concatKeys[$key] = true;
    }

    /**
     * Generates the concatenation code.
     *
     * @return array
     */
    abstract public function genConcatCode(): array;

    /**
     * Obtains the existing concatenation keys.
     *
     * @return array
     */
    public function getConcatKeys(): array
    {
        return $this->concatKeys;
    }
}
