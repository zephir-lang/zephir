<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team                                  |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the MIT license,       |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the MIT license and are unable      |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

namespace Zephir;

/**
 * ClassMethodParameters
 *
 * Represents the parameters defined in a method
 */
class ClassMethodParameters implements \Countable, \Iterator
{
    private $_parameters = array();

    private $_position = 0;

    /**
     *
     * @param array $parameters
     */
    public function __construct(array $parameters)
    {
        // @todo: Remove this
        foreach ($parameters as $parameter) {
            if (isset($parameter['reference']) && $parameter['reference']) {
                throw new CompilerException('Zephir not support reference parameters for now. Stay tuned for https://github.com/phalcon/zephir/issues/203', $parameter);
            }
        }
        $this->_parameters = $parameters;
    }

    /**
     * Return internal parameters
     *
     * @return array
     */
    public function getParameters()
    {
        return $this->_parameters;
    }

    /**
     * @return int
     */
    public function count()
    {
        return count($this->_parameters);
    }

    public function rewind()
    {
        $this->_position = 0;
    }

    public function key()
    {
        return $this->_position;
    }

    public function valid()
    {
        return isset($this->_parameters[$this->_position]);
    }

    public function current()
    {
        return $this->_parameters[$this->_position];
    }

    public function next()
    {
        $this->_position++;
    }
}
