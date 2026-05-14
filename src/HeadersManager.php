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
class HeadersManager
{
    /**
     * Insert the header at the beginning of the header list.
     */
    public const POSITION_FIRST = 1;

    /**
     * Insert the header at the end of the header list.
     */
    public const POSITION_LAST = 2;

    /**
     * List of headers.
     */
    protected array $headers = [];

    /**
     * List of headers.
     */
    protected array $headersFirst = [];

    /**
     * List of headers.
     */
    protected array $headersLast = [];

    /**
     * Adds a header path to the manager.
     *
     * @throws InvalidArgumentException
     */
    public function add(string $path, int $position = 0): void
    {
        if (!$position) {
            $this->headers[$path] = $path;
        } elseif ($position === self::POSITION_FIRST) {
            $this->headersFirst[$path] = $path;
        } elseif ($position === self::POSITION_LAST) {
            $this->headersLast[$path] = $path;
        }
    }

    /**
     * Returns a set of headers merged.
     */
    public function get(): array
    {
        return array_merge($this->headersFirst, $this->headers, $this->headersLast);
    }
}
