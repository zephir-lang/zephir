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

namespace Zephir\Builder;

/**
 * Allows to manually build a statements block AST node
 */
class StatementsBlockBuilder
{
    protected array $statements;

    protected bool $raw;

    public function __construct(array $statements, bool $raw = false)
    {
        $this->statements = $statements;
        $this->raw = $raw;
    }

    /**
     * Returns a builder definition.
     *
     * @return array
     */
    public function get(): array
    {
        if ($this->raw) {
            return $this->statements;
        }

        $statements = [];
        foreach ($this->statements as $statement) {
            $statements[] = $statement->get();
        }

        return $statements;
    }
}
