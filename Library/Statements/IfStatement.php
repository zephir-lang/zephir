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
use Zephir\Passes\SkipVariantInit;
use Zephir\StatementsBlock;
use Zephir\Optimizers\EvalExpression;
use Zephir\Branch;

/**
 * IfStatement
 *
 * 'If' statement, the same as in PHP/C
 */
class IfStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     */
    public function compile(CompilationContext $compilationContext)
    {
        /**
         * This pass tries to move dynamic variable initialization out of the if/else branch
         */
        if (isset($this->_statement['statements']) && isset($this->_statement['else_statements'])) {

            $skipVariantInit = new SkipVariantInit();

            $skipVariantInit->pass(0, new StatementsBlock($this->_statement['statements']));
            $skipVariantInit->pass(1, new StatementsBlock($this->_statement['else_statements']));

            $symbolTable = $compilationContext->symbolTable;
            foreach ($skipVariantInit->getVariables() as $variable) {
                if ($symbolTable->hasVariable($variable)) {
                    $symbolVariable = $symbolTable->getVariable($variable);
                    if ($symbolVariable->getType() == 'variable') {
                        $symbolVariable->initVariant($compilationContext);
                        $symbolVariable->skipInitVariant(2);
                    }
                }
            }
        }

        $exprRaw = $this->_statement['expr'];

        $expr = new EvalExpression();
        $condition = $expr->optimize($exprRaw, $compilationContext);
        $compilationContext->codePrinter->output('if (' . $condition . ') {');
        $this->_evalExpression = $expr;

        /**
         * Try to mark lastest temporary variable used as idle
         */
        $evalVariable =  $expr->getEvalVariable();
        if (is_object($evalVariable)) {
            if ($evalVariable->isTemporal()) {
                $evalVariable->setIdle(true);
            }
        }

        /**
         * Compile statements in the 'if' block
         */
        if (isset($this->_statement['statements'])) {
            $st = new StatementsBlock($this->_statement['statements']);
            $branch = $st->compile($compilationContext, $expr->isUnrecheable(), Branch::TYPE_CONDITIONAL_TRUE);
            $branch->setRelatedStatement($this);
        }

        /**
         * Compile statements in the 'else' block
         */
        if (isset($this->_statement['else_statements'])) {
            $compilationContext->codePrinter->output('} else {');
            $st = new StatementsBlock($this->_statement['else_statements']);
            $branch = $st->compile($compilationContext, $expr->isUnrecheableElse(), Branch::TYPE_CONDITIONAL_FALSE);
            $branch->setRelatedStatement($this);
        }

        $compilationContext->codePrinter->output('}');

    }
}
