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

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\Expression;

/**
 * RequireStatement
 *
 * Require statement is used to execute PHP scripts in a given path
 */
class RequireStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        $expression = array(
            'type' => 'require',
            'left' => $this->_statement['expr'],
            'file' => $this->_statement['file'],
            'line' => $this->_statement['line'],
            'char' => $this->_statement['char']
        );

        $expr = new Expression($expression);
        $expr->setExpectReturn(false, null);
        $expr->compile($compilationContext);
    }
}
