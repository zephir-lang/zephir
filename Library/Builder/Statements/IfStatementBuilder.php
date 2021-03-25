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

use Zephir\Builder\Operators\AbstractOperatorBuilder;
use Zephir\Builder\StatementsBlockBuilder;

/**
 * IfStatementBuilder.
 *
 * Allows to manually build a 'if' statement AST node
 */
class IfStatementBuilder extends AbstractStatementBuilder
{
    private $evalExpr;

    private $ifBlock;

    private $elseBlock;

    /**
     * IfStatementBuilder constructor.
     *
     * @param AbstractOperatorBuilder $evalExpr
     * @param StatementsBlockBuilder  $ifBlock
     * @param StatementsBlockBuilder  $elseBlock
     */
    public function __construct(AbstractOperatorBuilder $evalExpr, StatementsBlockBuilder $ifBlock, StatementsBlockBuilder $elseBlock = null)
    {
        $this->evalExpr = $evalExpr;
        $this->ifBlock = $ifBlock;
        $this->elseBlock = $elseBlock;
    }

    /**
     * {@inheritdoc}
     */
    public function get()
    {
        $expression = [
            'type' => 'if',
            'expr' => $this->evalExpr->get(),
            'statements' => $this->ifBlock->get(),
        ];

        if ($this->elseBlock) {
            $expression['else_statements'] = $this->elseBlock->get();
        }

        return $expression;
    }
}
