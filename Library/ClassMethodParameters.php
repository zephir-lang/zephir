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

use ArrayAccess;
use Countable;
use Iterator;
use Zephir\Exception\CompilerException;

use function count;

/**
 * Class Method Parameters
 *
 * Represents the parameters defined in a method.
 */
class ClassMethodParameters implements Countable, Iterator, ArrayAccess
{
    /**
     * List of all method's parameters
     *
     * @var array
     */
    private array $parameters = [];

    private int $position = 0;

    /**
     * List of Required parameters
     *
     * @var array
     */
    private array $requiredParameters = [];

    /**
     * List of Optional parameters (with default value)
     *
     * @var array
     */
    private array $optionalParameters = [];

    /**
     * ClassMethodParameters constructor.
     *
     * @param array $parameters
     */
    public function __construct(array $parameters)
    {
        $this->parameters = $parameters;
    }

    /**
     * Return internal parameters.
     *
     * @return array
     */
    public function getParameters(): array
    {
        return $this->parameters;
    }

    public function getRequiredParameters(): array
    {
        return $this->requiredParameters;
    }

    public function countRequiredParameters(): int
    {
        return count($this->requiredParameters);
    }

    public function getOptionalParameters(): array
    {
        return $this->optionalParameters;
    }

    public function countOptionalParameters(): int
    {
        return count($this->optionalParameters);
    }

    public function fetchParameters(bool $isMethodInternal): array
    {
        $parameters = [];

        foreach ($this->parameters as $parameter) {
            $name = $parameter['name'];
            $dataType = $parameter['data-type'] ?? 'variable';

            switch ($dataType) {
                case 'object':
                case 'callable':
                case 'resource':
                case 'variable':
                    $parameters[] = $isMethodInternal ? $name : '&'.$name;
                    break;

                default:
                    $parameters[] = ($isMethodInternal ? $name : '&'.$name).'_param';
                    break;
            }

            if (isset($parameter['default'])) {
                $this->optionalParameters[] = $parameter;
            } else {
                $this->requiredParameters[] = $parameter;
            }
        }

        return $parameters;
    }

    /**
     * @return int
     */
    public function count(): int
    {
        return count($this->parameters);
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
