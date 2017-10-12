<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Builder\Statements;

use Zephir\Builder\StatementsBlockBuilder;
use Zephir\Builder\Operators\AbstractOperatorBuilder;

/**
 * IfStatementBuilder
 *
 * Allows to manually build a 'if' statement AST node
 */
class IfStatementBuilder extends AbstractStatementBuilder
{
    private $evalExpr;

    private $ifBlock;

    private $elseBlock;

    /**
     * IfStatementBuilder constructor
     *
     * @param AbstractOperatorBuilder $evalExpr
     * @param StatementsBlockBuilder $ifBlock
     * @param StatementsBlockBuilder $elseBlock
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
        $expression = array(
            'type' => 'if',
            'expr' => $this->evalExpr->get(),
            'statements' => $this->ifBlock->get()
        );

        if ($this->elseBlock) {
            $expression['else_statements'] = $this->elseBlock->get();
        }

        return $expression;
    }
}
