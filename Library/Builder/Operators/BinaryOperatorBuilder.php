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
