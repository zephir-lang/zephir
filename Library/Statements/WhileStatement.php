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
use Zephir\Optimizers\EvalExpression;
use Zephir\StatementsBlock;

/**
 * WhileStatement
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
        $exprRaw = $this->_statement['expr'];
        $codePrinter = $compilationContext->codePrinter;

        /**
         * Compound conditions can be evaluated in a single line of the C-code
         */
        $codePrinter->output('while (1) {');

        $codePrinter->increaseLevel();

        /**
         * Variables are initialized in a different way inside loops
         */
        $compilationContext->insideCycle++;

        $expr = new EvalExpression();
        $condition = $expr->optimize($exprRaw, $compilationContext);
        $this->_evalExpression = $expr;

        $codePrinter->output('if (!(' . $condition . ')) {');
        $codePrinter->output("\t" . 'break;');
        $codePrinter->output('}');

        $codePrinter->decreaseLevel();

        /**
         * Compile statements in the 'while' block
         */
        if (isset($this->_statement['statements'])) {
            $st = new StatementsBlock($this->_statement['statements']);
            $st->isLoop(true);
            $st->compile($compilationContext);
        }

        /**
         * Restore the cycle counter
         */
        $compilationContext->insideCycle--;

        $codePrinter->output('}');
    }
}
