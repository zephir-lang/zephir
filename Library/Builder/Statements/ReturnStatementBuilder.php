<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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
 * ReturnStatementBuilder
 *
 * Allows to manually build a 'return' statement AST node
 */
class ReturnStatementBuilder extends AbstractStatementBuilder
{
    private $expr;

    /**
     * ReturnStatementBuilder constructor
     *
     * @param mixed $expr
     */
    public function __construct($expr)
    {
        $this->expr = $expr;
    }

    /**
     * {@inheritdoc}
     */
    public function get()
    {
        return array(
            'type' => 'return',
            'expr' => $this->expr->get()
        );
    }
}
