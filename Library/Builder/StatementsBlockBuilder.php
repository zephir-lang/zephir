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
    public function __construct(protected array $statements, protected bool $raw = false)
    {
    }

    /**
     * Returns a builder definition.
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
