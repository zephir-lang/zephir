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

use Zephir\Builder\Operators\AbstractOperatorBuilder;

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
