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
 * Allows to manually build a 'let' statement AST node
 */
class LetStatementBuilder extends AbstractStatementBuilder
{
    /**
     * List of assignments
     *
     * @var array
     */
    private array $assignments;

    /**
     * Expression
     *
     * @var mixed
     */
    private $expr;

    public function __construct(array $assignments, $expr)
    {
        $this->assignments = $assignments;
        $this->expr = $expr;
    }

    /**
     * Returns a builder definition.
     *
     * @return array
     */
    public function get(): array
    {
        return [
            'type' => 'let',
            'assignments' => [array_merge(['expr' => $this->expr], $this->assignments)],
        ];
    }
}
