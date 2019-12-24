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
 * IfStatementBuilder.
 *
 * Allows to manually build a 'let' statement AST node
 */
class LetStatementBuilder extends AbstractStatementBuilder
{
    private $expr;

    private $assignments;

    public function __construct($assignments, $expr)
    {
        $this->assignments = $assignments;
        $this->expr = $expr;
    }

    /**
     * {@inheritdoc}
     */
    public function get()
    {
        return [
            'type' => 'let',
            'assignments' => [array_merge(['expr' => $this->expr], $this->assignments)],
        ];
    }
}
