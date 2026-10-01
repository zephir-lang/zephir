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

namespace Zephir\Code\Builder;

use Zephir\Exception\InvalidArgumentException;

use function sprintf;
use function substr;

use const PHP_EOL;

/**
 * Represents an internal extension global structure.
 *
 * Declares the C type only. Everything else about a member - its php.ini
 * directive, its compiled default - comes from ExtensionGlobal, so a dotted
 * global and a plain one cannot be emitted differently.
 */
class Struct
{
    /**
     * Struct members definition
     *
     * ```c
     * struct Name {
     *     key value;
     * }
     * ```
     */
    protected array $properties = [];

    public function __construct(protected string $name, protected string $simpleName)
    {
        if (empty($name)) {
            throw new InvalidArgumentException('Struct name must not be empty');
        }
    }

    public function __toString(): string
    {
        $code = 'typedef struct ' . $this->name . ' { ' . PHP_EOL;

        foreach ($this->properties as $name => $type) {
            $code .= sprintf("\t%s %s;%s", $type, $name, PHP_EOL);
        }

        return $code . '} ' . substr($this->name, 1) . ';' . PHP_EOL;
    }

    /**
     * @throws InvalidArgumentException
     */
    public function addProperty(string $field, string $type): void
    {
        if (isset($this->properties[$field])) {
            throw new InvalidArgumentException('Property was defined more than once');
        }

        $this->properties[$field] = ExtensionGlobal::cTypeOf($type);
    }
}
