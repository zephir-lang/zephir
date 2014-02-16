<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Builder\Statements;

/**
 * IfStatementBuilder
 *
 * Allows to manually build a 'if' statement AST node
 */
class IfStatementBuilder
{
    private $evalExpr;

    private $ifBlock;

    private $elseBlock;

    /**
     * IfStatementBuilder constructor
     *
     * @param array $evalExpr
     * @param array $ifBlock
     * @param array $elseBlock
     */
    public function __construct(array $evalExpr, array $ifBlock, $elseBlock=null)
    {
        $this->evalExpr = $evalExpr;
        $this->ifBlock = $ifBlock;
        $this->elseBlock = $elseBlock;
    }

    public function get()
    {
        return array(
            'type' => 'if',
            'expr' => $this->evalExpr,
            'statements' => $this->ifBlock
        );
    }
}
