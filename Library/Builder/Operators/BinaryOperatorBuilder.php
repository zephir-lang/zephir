<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder\Operators;

/**
 * BinaryOperatorBuilder.
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
     * BinaryOperatorBuilder constructor.
     *
     * @param string  $operator
     * @param Builder $leftExpression
     * @param Builder $rightExpression
     * @param string  $file
     * @param int     $line
     * @param string  $char
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
     * Returns a builder definition.
     *
     * @return array
     */
    public function get()
    {
        return [
            'type' => $this->operator,
            'left' => $this->leftExpression->get(),
            'right' => $this->rightExpression->get(),
            'file' => $this->file,
            'line' => $this->line,
            'char' => $this->char,
        ];
    }
}
