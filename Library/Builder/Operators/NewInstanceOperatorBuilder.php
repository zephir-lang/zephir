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

namespace Zephir\Builder\Operators;

use Zephir\Builder\Operators\AbstractOperatorBuilder;

/**
 * NewInstanceOperatorBuilder
 *
 * Allows to manually build a 'new instance' operator AST node
 */
class NewInstanceOperatorBuilder extends AbstractOperatorBuilder
{
    protected $type;

    protected $className;

    protected $dynamic;

    protected $file;

    protected $line;

    protected $char;

    public function __construct($className, array $parameters, $dynamic = false, $file = null, $line = 0, $char = 0)
    {
        $this->className = $className;
        $this->parameters = $parameters;
        $this->dynamic = $dynamic;
        $this->file = $file;
        $this->line = $line;
        $this->char = $char;
    }

    /**
     * Returns a builder definition
     *
     * @return array
     */
    public function get()
    {
        $parameters = array();
        foreach ($this->parameters as $parameter) {
            $parameters[] = $parameter->get();
        }
        return array(
            'type'       => 'new',
            'class'      => $this->className,
            'parameters' => $parameters,
            'dynamic'    => $this->dynamic,
            'file'       => $this->file,
            'line'       => $this->line,
            'char'       => $this->char
        );
    }
}
