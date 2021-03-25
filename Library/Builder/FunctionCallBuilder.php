<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder;

/**
 * FunctionCallBuilder.
 *
 * Allows to manually build a function call AST node
 */
class FunctionCallBuilder
{
    protected $name;

    protected $parameters;

    protected $type;

    protected $file;

    protected $line;

    protected $char;

    /**
     * FunctionCallBuilder construct.
     *
     * @param string $name
     * @param array  $parameters
     * @param int    $type
     * @param string $file
     * @param int    $line
     * @param int    $char
     */
    public function __construct($name, $parameters, $type = 1, $file = null, $line = 0, $char = 0)
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
    public function get()
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
