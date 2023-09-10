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

namespace Zephir\Class\Method;

use ArrayAccess;
use Countable;
use Iterator;
use Zephir\Exception\CompilerException;
use function count;

/**
 * Represents the parameters defined in a method.
 */
class Parameters implements Countable, Iterator, ArrayAccess
{
    private int $position = 0;

    /**
     * List of Required parameters
     */
    private array $requiredParameters = [];

    /**
     * List of Optional parameters (with default value)
     */
    private array $optionalParameters = [];

    /**
     * @throws CompilerException
     */
    public function __construct(private array $parameters)
    {
        foreach ($parameters as $parameter) {
            if (isset($parameter['reference']) && $parameter['reference']) {
                throw new CompilerException(
                    'Zephir not support reference parameters for now. '.
                    'Stay tuned for https://github.com/zephir-lang/zephir/issues/203',
                    $parameter
                );
            }
        }
    }

    /**
     * Return internal parameters.
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
                case 'mixed':
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

    public function rewind(): void
    {
        $this->position = 0;
    }

    public function key(): int
    {
        return $this->position;
    }

    public function valid(): bool
    {
        return isset($this->parameters[$this->position]);
    }

    public function current(): mixed
    {
        return $this->parameters[$this->position];
    }

    public function next(): void
    {
        ++$this->position;
    }

    public function offsetExists(mixed $offset): bool
    {
        return isset($this->parameters[$offset]);
    }

    public function offsetGet(mixed $offset): mixed
    {
        return $this->parameters[$offset];
    }

    public function offsetSet(mixed $offset, mixed $value): void
    {
        $this->parameters[$offset] = $value;
    }

    public function offsetUnset(mixed $offset): void
    {
        unset($this->parameters[$offset]);
    }
}
