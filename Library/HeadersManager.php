<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

    const POSITION_FIRST = 1;
    const POSITION_LAST = 2;

    /**
     * List of headers
     * @var array
     */
    protected $_headers = array();

    /**
     * List of headers
     * @var array
     */
    protected $_headersFirst = array();

    /**
     * List of headers
     * @var array
     */
    protected $_headersLast = array();

    /**
     * @param string $path
     */
    public function add($path, $position = 0)
    {
        if (!is_string($path)) {
            throw new \InvalidArgumentException('$path must be only string type');
        }
        if (!$position) {
            $this->_headers[$path] = $path;
        } else {
            switch ($position) {

                case self::POSITION_FIRST:
                    $this->_headersFirst[$path] = $path;
                    break;

                case self::POSITION_LAST:
                    $this->_headersLast[$path] = $path;
                    break;

                default:
                    break;
            }
        }
    }

    /**
     * @return array
     */
    public function get()
    {
        return array_merge($this->_headersFirst, $this->_headers, $this->_headersLast);
    }
}
