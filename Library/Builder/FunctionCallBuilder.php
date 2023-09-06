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
    /**
     * @param string      $name
     * @param array       $parameters
     * @param int         $type
     * @param string|null $file
     * @param int         $line
     * @param int         $char
     */
    public function __construct(
        protected string $name,
        protected array $parameters,
        protected int $type = 1,
        protected ?string $file = null,
        protected int $line = 0,
        protected int $char = 0,
    ) {
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
