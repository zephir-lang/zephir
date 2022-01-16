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

namespace Zephir\Builder;

/**
 * Allows to manually build a function call AST node
 */
class FunctionCallBuilder
{
    protected string $name;

    protected array $parameters;

    protected int $type;

    protected ?string $file;

    protected int $line = 0;

    protected int $char = 0;

    /**
     * @param string      $name
     * @param array       $parameters
     * @param int         $type
     * @param string|null $file
     * @param int         $line
     * @param int         $char
     */
    public function __construct(string $name, array $parameters, int $type = 1, ?string $file = null, int $line = 0, int $char = 0)
    {
        $this->name = $name;
        $this->parameters = $parameters;
        $this->type = $type;
        $this->file = $file;
        $this->line = $line;
        $this->char = $char;
    }

    /**
     * @return array
     */
    public function get(): array
    {
        return [
            'type' => 'fcall',
            'name' => $this->name,
            'call-type' => $this->type,
            'parameters' => $this->parameters,
            'file' => $this->file,
            'line' => $this->line,
            'char' => $this->char,
        ];
    }
}
