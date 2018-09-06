<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Builder\Operators;

/**
 * TypeOfOperatorBuilder
 *
 * Allows to manually build a 'typeof' operator AST node
 */
class TypeOfOperatorBuilder extends AbstractOperatorBuilder
{
    protected $leftOperand;

    /**
     * @param $left
     * @param null $file
     * @param int $line
     * @param int $char
     */
    public function __construct($left, $file = null, $line = 0, $char = 0)
    {
        $this->leftOperand = $left;
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
        return array(
            'type'       => 'typeof',
            'left'       => $this->leftOperand->get(),
            'file'       => $this->file,
            'line'       => $this->line,
            'char'       => $this->char
        );
    }
}
