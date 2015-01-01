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
use Zephir\Optimizers\EvalExpression;
use Zephir\StatementsBlock;

/**
 * DoWhileStatement
 *
 * DoWhile statement, the same as in PHP/C
 */
class DoWhileStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     */
    public function compile(CompilationContext $compilationContext)
    {
        $exprRaw = $this->_statement['expr'];
        $codePrinter = $compilationContext->codePrinter;

        $codePrinter->output('do {');

        /**
         * Variables are initialized in a different way inside cycle
         */
        $compilationContext->insideCycle++;

        /**
         * Compile statements in the 'while' block
         */
        if (isset($this->_statement['statements'])) {
            $st = new StatementsBlock($this->_statement['statements']);
            $st->compile($compilationContext);
        }

        $compilationContext->codePrinter->increaseLevel();
        $expr = new EvalExpression();
        $condition = $expr->optimize($exprRaw, $compilationContext);
        $compilationContext->codePrinter->decreaseLevel();

        /**
         * Restore the cycle counter
         */
        $compilationContext->insideCycle--;

        /**
         * Compound conditions can be evaluated in a single line of the C-code
         */
        $codePrinter->output('} while (' . $condition . ');');
    }
}
