<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder\Statements;

/**
 * ReturnStatementBuilder.
 *
 * Allows to manually build a 'return' statement AST node
 */
class ReturnStatementBuilder extends AbstractStatementBuilder
{
    private $expr;

    /**
     * ReturnStatementBuilder constructor.
     *
     * @param mixed $expr
     */
    public function __construct($expr)
    {
        $this->expr = $expr;
    }

    /**
     * {@inheritdoc}
     */
    public function get()
    {
        return [
            'type' => 'return',
            'expr' => $this->expr->get(),
        ];
    }
}
