<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
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
