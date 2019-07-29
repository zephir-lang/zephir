<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Support;

use Zephir\Exception\InvalidCallException;
use Zephir\Exception\UnknownPropertyException;

/**
 * Zephir\Support\PropertyAccessor.
 */
trait PropertyAccessor
{
    /**
     * Gets the internal Object's property.
     *
     * Do not call this method directly as it is a PHP magic method that
     * will be implicitly called when executing `$value = $object->property;`.
     *
     * @param string $name
     *
     * @throws InvalidCallException
     * @throws UnknownPropertyException
     *
     * @return mixed
     */
    public function __get($name)
    {
        $getter = 'get'.ucfirst($name);
        $setter = 'set'.ucfirst($name);

        if (method_exists($this, $getter)) {
            return $this->$getter();
        } elseif (method_exists($this, $setter)) {
            throw new InvalidCallException(
                sprintf('Getting write-only property: %s::%s', \get_class($this), $name)
            );
        }

        throw new UnknownPropertyException(
            sprintf('Getting unknown property: %s::%s', \get_class($this), $name)
        );
    }
}
