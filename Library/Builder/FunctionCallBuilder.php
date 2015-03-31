<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

namespace Zephir\Builder;

/**
 * FunctionCallBuilder
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
     * FunctionCallBuilder construct
     *
     * @param string $name
     * @param array $parameters
     * @param int $type
     * @param string $file
     * @param int $line
     * @param int $char
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
        return array(
            'type'       => 'fcall',
            'name'       => $this->name,
            'call-type'  => $this->type,
            'parameters' => $this->parameters,
            'file'       => $this->file,
            'line'       => $this->line,
            'char'       => $this->char
        );
    }
}
