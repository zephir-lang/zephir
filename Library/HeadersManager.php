<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

/**
 * Class HeadersManager
 *
 * Manages the c-headers that must be added to a file
 */
class HeadersManager
{
    /**
     * Insert the header at the beginning of the header list
     */
    const POSITION_FIRST = 1;

    /**
     * Insert the header at the end of the header list
     */
    const POSITION_LAST = 2;

    /**
     * List of headers
     * @var array
     */
    protected $headers = array();

    /**
     * List of headers
     * @var array
     */
    protected $headersFirst = array();

    /**
     * List of headers
     * @var array
     */
    protected $headersLast = array();

    /**
     * Adds a header path to the manager
     *
     * @param string $path
     * @param int $position
     * @throws \InvalidArgumentException
     */
    public function add($path, $position = 0)
    {
        if (!is_string($path)) {
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
     * Returns a set of headers merged
     *
     * @return array
     */
    public function get()
    {
        return array_merge($this->headersFirst, $this->headers, $this->headersLast);
    }
}
