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

use Zephir\Builder\Operators\AbstractOperatorBuilder;
use Zephir\Builder\StatementsBlockBuilder;

/**
 * Allows to manually build a 'if' statement AST node
 */
class IfStatementBuilder extends AbstractStatementBuilder
{
    public function __construct(
        private AbstractOperatorBuilder $evalExpr,
        private StatementsBlockBuilder $ifBlock,
        private ?StatementsBlockBuilder $elseBlock = null
    ) {
    }

    public function get(): array
    {
        $expression = [
            'type'       => 'if',
            'expr'       => $this->evalExpr->get(),
            'statements' => $this->ifBlock->get(),
        ];

        if ($this->elseBlock) {
            $expression['else_statements'] = $this->elseBlock->get();
        }

        return $expression;
    }
}
