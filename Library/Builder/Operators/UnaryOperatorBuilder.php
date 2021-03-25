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
 * UnaryOperatorBuilder.
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
     * Returns a builder definition.
     *
     * @return array
     */
    public function get()
    {
        if (\is_object($this->leftExpression) && method_exists($this->leftExpression, 'get')) {
            $expr = $this->leftExpression->get();
        } else {
            $expr = $this->leftExpression;
        }

        return [
            'type' => $this->operator,
            'left' => $expr,
            'file' => $this->file,
            'line' => $this->line,
            'char' => $this->char,
        ];
    }
}
