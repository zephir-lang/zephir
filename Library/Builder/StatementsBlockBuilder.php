<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder;

/**
 * StatementsBlockBuilder.
 *
 * Allows to manually build a statements block AST node
 */
class StatementsBlockBuilder
{
    protected $statements;

    protected $raw;

    public function __construct(array $statements, $raw = false)
    {
        $this->statements = $statements;
        $this->raw = $raw;
    }

    /**
     * Returns a builder definition.
     *
     * @return array
     */
    public function get()
    {
        if (!$this->raw) {
            $statements = [];

            foreach ($this->statements as $statement) {
                $statements[] = $statement->get();
            }

            return $statements;
        }

        return $this->statements;
    }
}
