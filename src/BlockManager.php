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

use InvalidArgumentException;

use function array_merge;

/**
 * Manages the c-headers that must be added to a file
 */
class BlockManager
{
    /**
     * List of headers.
     */
    protected array $blocks = [];

    /**
     * Adds a header path to the manager.
     *
     * @throws InvalidArgumentException
     */
    #[SuppressWarnings("php:S4790")]
    public function add(string $block): BlockManager
    {
        $this->blocks[md5($block)] = $block;

        return $this;
    }

    /**
     * Returns a set of headers merged.
     */
    public function get(): array
    {
        return $this->blocks;
    }

    public function isEmpty(): bool
    {
        return empty($this->blocks);
    }
}
