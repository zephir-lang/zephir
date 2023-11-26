<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

/**
 * Manages the c-headers that must be added to a file
 */
class HeadersManager
{
    /**
     * Insert the header at the beginning of the header list.
     */
    const POSITION_FIRST = 1;

    /**
     * Insert the header at the end of the header list.
     */
    const POSITION_LAST = 2;

    /**
     * List of headers.
     *
     * @var array
     */
    protected array $headers = [];

    /**
     * List of headers.
     *
     * @var array
     */
    protected array $headersFirst = [];

    /**
     * List of headers.
     *
     * @var array
     */
    protected array $headersLast = [];

    /**
     * Adds a header path to the manager.
     *
     * @param string $path
     * @param int    $position
     *
     * @throws \InvalidArgumentException
     */
    public function add($path, $position = 0): void
    {
        if (!\is_string($path)) {
            throw new \InvalidArgumentException('$path must be only string type');
        }

        if (!$position) {
            $this->headers[$path] = $path;
        } else {
            switch ($position) {
                case self::POSITION_FIRST:
                    $this->headersFirst[$path] = $path;
                    break;
                case self::POSITION_LAST:
                    $this->headersLast[$path] = $path;
                    break;
                default:
                    break;
            }
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
