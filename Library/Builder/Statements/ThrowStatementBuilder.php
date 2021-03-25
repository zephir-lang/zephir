<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder\Statements;

/**
 * ThrowStatementBuilder.
 *
 * Allows to manually build a 'throw' statement AST node
 */
class ThrowStatementBuilder extends AbstractStatementBuilder
{
    private $expr;

    /**
     * ThrowStatementBuilder constructor.
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
            'type' => 'throw',
            'expr' => $this->expr->get(),
        ];
    }
}
