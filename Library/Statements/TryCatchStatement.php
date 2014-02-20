<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
 * TryCatchStatement
 *
 * Try/Catch statement the same as in PHP
 */
class TryCatchStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     */
    public function compile(CompilationContext $compilationContext)
    {
        $codePrinter = &$compilationContext->codePrinter;

        $compilationContext->insideTryCatch++;

        $codePrinter->outputBlankLine();
        $codePrinter->output('/* try_start_' . $compilationContext->insideTryCatch . ': */');
        $codePrinter->outputBlankLine();

        if (isset($this->_statement['statements'])) {
            $st = new StatementsBlock($this->_statement['statements']);
            $st->compile($compilationContext);
        }

        $codePrinter->outputBlankLine();
        $codePrinter->output('try_end_' . $compilationContext->insideTryCatch . ':');
        $codePrinter->output('do { } while(0);');
        $codePrinter->outputBlankLine();

        if (isset($this->_statement['catches'])) {
            foreach ($this->_statement['catches'] as $catch) {

                if (isset($catch['variable'])) {
                    $variable = $compilationContext->symbolTable->getVariableForWrite($catch['variable']['value'], $compilationContext, $compilationContext, $catch['variable']);
                }

            }
        }

        $compilationContext->insideTryCatch--;

    }
}
