<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Optimizers\EvalExpression;
use Zephir\StatementsBlock;

/**
 * WhileStatement.
 *
 * While statement, the same as in PHP/C
 */
class WhileStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     */
    public function compile(CompilationContext $compilationContext)
    {
        $exprRaw = $this->statement['expr'];
        $codePrinter = $compilationContext->codePrinter;

        /*
         * Compound conditions can be evaluated in a single line of the C-code
         */
        $codePrinter->output('while (1) {');

        $codePrinter->increaseLevel();

        /*
         * Variables are initialized in a different way inside loops
         */
        ++$compilationContext->insideCycle;

        $expr = new EvalExpression();
        $condition = $expr->optimize($exprRaw, $compilationContext);
        $this->evalExpression = $expr;

        $codePrinter->output('if (!('.$condition.')) {');
        $codePrinter->output("\t".'break;');
        $codePrinter->output('}');

        $codePrinter->decreaseLevel();

        /*
         * Compile statements in the 'while' block
         */
        if (isset($this->statement['statements'])) {
            $st = new StatementsBlock($this->statement['statements']);
            $st->isLoop(true);
            $st->compile($compilationContext);
        }

        /*
         * Restore the cycle counter
         */
        --$compilationContext->insideCycle;

        $codePrinter->output('}');
    }
}
