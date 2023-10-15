<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Builder\Operators;

/**
 * Allows to manually build a unary operator AST node
 */
class UnaryOperatorBuilder extends AbstractOperatorBuilder
{
    /**
     * Operator name
     */
    protected string $operator;

    /**
     * Left Expression
     *
     * @var mixed
     */
    protected $leftExpression;

    public function __construct(string $operator, $leftExpression, string $file = null, int $line = 0, int $char = 0)
    {
        $this->operator = $operator;
        $this->leftExpression = $leftExpression;
        $this->file = $file;
        $this->line = $line;
        $this->char = $char;
    }

    public function get(): array
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
