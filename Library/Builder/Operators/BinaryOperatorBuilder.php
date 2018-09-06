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
 * BinaryOperatorBuilder
 *
 * Allows to manually build a binary operator AST node
 */
class BinaryOperatorBuilder extends AbstractOperatorBuilder
{
    protected $operator;

    protected $file;

    protected $line;

    protected $char;

    /**
     * BinaryOperatorBuilder constructor
     *
     * @param string $operator
     * @param Builder $leftExpression
     * @param Builder $rightExpression
     * @param string $file
     * @param int $line
     * @param string $char
     */
    public function __construct($operator, $leftExpression, $rightExpression, $file = null, $line = 0, $char = 0)
    {
        $this->operator = $operator;
        $this->leftExpression = $leftExpression;
        $this->rightExpression = $rightExpression;
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
            'type'       => $this->operator,
            'left'       => $this->leftExpression->get(),
            'right'      => $this->rightExpression->get(),
            'file'       => $this->file,
            'line'       => $this->line,
            'char'       => $this->char
        );
    }
}
