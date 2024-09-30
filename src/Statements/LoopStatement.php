<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Statements;

use ReflectionException;
use Zephir\Branch;
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Passes\LoopBreakPass;
use Zephir\StatementsBlock;

/**
 * Loop statement, infinite loop
 */
class LoopStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $compilationContext->codePrinter->output('while (1) {');

        /**
         * Variables are initialized in a different way inside cycle
         */
        ++$compilationContext->insideCycle;

        /**
         * Compile statements in the 'loop' block
         */
        if (!isset($this->statement['statements'])) {
            throw new CompilerException(
                "Infinite loop without at least a 'break' statement is not allowed",
                $this->statement
            );
        }

        $st = new StatementsBlock($this->statement['statements']);

        /**
         * Check if the block contain at least a break statement.
         */
        $loopBreakPass = new LoopBreakPass();
        $loopBreakPass->pass($st);
        if (!$loopBreakPass->hasBreak()) {
            throw new CompilerException(
                "Infinite loop without at least a 'break' statement is not allowed",
                $this->statement
            );
        }

        $st->isLoop(true);
        $st->compile($compilationContext, false, Branch::TYPE_LOOP_INFINITE);

        --$compilationContext->insideCycle;

        $compilationContext->codePrinter->output('}');
    }
}
