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
 * CastOperatorBuilder
 *
 * Allows to manually build a 'cast' operator AST node
 */
class CastOperatorBuilder extends AbstractOperatorBuilder
{
    protected $leftOperand;

    protected $rightOperand;

    /**
     * @param $left
     * @param $right
     * @param null $file
     * @param int $line
     * @param int $char
     */
    public function __construct($left, $right, $file = null, $line = 0, $char = 0)
    {
        $this->leftOperand = $left;
        $this->rightOperand = $right;
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
            'type'       => 'cast',
            'left'       => $this->leftOperand,
            'right'      => $this->rightOperand->get(),
            'file'       => $this->file,
            'line'       => $this->line,
            'char'       => $this->char
        );
    }
}
