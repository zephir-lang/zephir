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

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\StatementsBlock;
use Zephir\Passes\LoopBreakPass;
use Zephir\Branch;

/**
 * LoopStatement
 *
 * Loop statement, infinite loop
 */
class LoopStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        $compilationContext->codePrinter->output('while (1) {');

        /**
         * Variables are initialized in a different way inside cycle
         */
        $compilationContext->insideCycle++;

        /**
         * Compile statements in the 'loop' block
         */
        if (!isset($this->_statement['statements'])) {
            throw new CompilerException("Infinite loop without at least a 'break' statement is not allowed", $this->_statement);
        }

        $st = new StatementsBlock($this->_statement['statements']);

        /**
         * Check if the block contain at least a break statement
         */
        $loopBreakPass = new LoopBreakPass();
        $loopBreakPass->pass($st);
        if (!$loopBreakPass->hasBreak()) {
            throw new CompilerException("Infinite loop without at least a 'break' statement is not allowed", $this->_statement);
        }

        $st->isLoop(true);
        $st->compile($compilationContext, false, Branch::TYPE_LOOP_INFINITE);

        $compilationContext->insideCycle--;

        $compilationContext->codePrinter->output('}');
    }
}
