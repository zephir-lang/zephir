<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Zephir\Exception\CompilerException;

/**
 * Zephir\ClassMethodParameters.
 *
 * Represents the parameters defined in a method
 */
class ClassMethodParameters implements \Countable, \Iterator, \ArrayAccess
{
    private $parameters = [];

    private $position = 0;

    /**
     * ClassMethodParameters constructor.
     *
     * @param array $parameters
     *
     * @throws CompilerException
     */
    public function __construct(array $parameters)
    {
        foreach ($parameters as $parameter) {
            if (isset($parameter['reference']) && $parameter['reference']) {
                throw new CompilerException(
                    'Zephir not support reference parameters for now. '.
                    'Stay tuned for https://github.com/phalcon/zephir/issues/203',
                    $parameter
                );
            }
        }

        $this->parameters = $parameters;
    }

    /**
     * Return internal parameters.
     *
     * @return array
     */
    public function getParameters()
    {
        return $this->parameters;
    }

    /**
     * @return int
     */
    public function count()
    {
        return \count($this->parameters);
    }

    public function rewind()
    {
        $this->position = 0;
    }

    public function key()
    {
        return $this->position;
    }

    public function valid()
    {
        return isset($this->parameters[$this->position]);
    }

    public function current()
    {
        return $this->parameters[$this->position];
    }

    public function next()
    {
        ++$this->position;
    }

    public function offsetExists($offset)
    {
        return isset($this->parameters[$offset]);
    }

    public function offsetGet($offset)
    {
        return $this->parameters[$offset];
    }

    public function offsetSet($offset, $value)
    {
        $this->parameters[$offset] = $value;
    }

    public function offsetUnset($offset)
    {
        unset($this->parameters[$offset]);
    }
}
