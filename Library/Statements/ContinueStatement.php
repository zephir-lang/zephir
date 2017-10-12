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

/**
 * ContinueStatement
 *
 * Continue statement
 */
class ContinueStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        if ($compilationContext->insideCycle) {
            $compilationContext->codePrinter->output('continue;');
        } else {
            throw new CompilerException("Cannot use 'continue' outside of a loop", $this->_statement);
        }
    }
}
