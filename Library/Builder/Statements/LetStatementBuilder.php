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
 * IfStatementBuilder
 *
 * Allows to manually build a 'let' statement AST node
 */
class LetStatementBuilder extends AbstractStatementBuilder
{
    private $expr;

    private $assignments;

    public function __construct($assignments, $expr)
    {
        $this->assignments = $assignments;
        $this->expr = $expr;
    }

    /**
     * {@inheritdoc}
     */
    public function get()
    {
        return array(
            'type' => 'let',
            'assignments' => array(array_merge(array('expr' => $this->expr), $this->assignments))
        );
    }
}
