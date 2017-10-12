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
 * UnaryOperatorBuilder
 *
 * Allows to manually build a unary operator AST node
 */
class UnaryOperatorBuilder extends AbstractOperatorBuilder
{
    protected $operator;

    protected $leftExpression;

    protected $file;

    protected $line;

    protected $char;

    public function __construct($operator, $leftExpression, $file = null, $line = 0, $char = 0)
    {
        $this->operator = $operator;
        $this->leftExpression = $leftExpression;
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
        if (is_object($this->leftExpression) && method_exists($this->leftExpression, 'get')) {
            $expr = $this->leftExpression->get();
        } else {
            $expr = $this->leftExpression;
        }

        return array(
            'type'       => $this->operator,
            'left'       => $expr,
            'file'       => $this->file,
            'line'       => $this->line,
            'char'       => $this->char
        );
    }
}
