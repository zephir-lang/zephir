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

namespace Zephir\Builder\Statements;

/**
 * Allows to manually build a 'let' statement AST node
 */
class LetStatementBuilder extends AbstractStatementBuilder
{
    public function __construct(private array $assignments, private mixed $expr)
    {
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
