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

/**
 * ThrowStatementBuilder
 *
 * Allows to manually build a 'throw' statement AST node
 */
class ThrowStatementBuilder extends AbstractStatementBuilder
{
    private $expr;

    /**
     * ThrowStatementBuilder constructor
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
            'type' => 'throw',
            'expr' => $this->expr->get()
        );
    }
}
