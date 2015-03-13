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
use Zephir\Expression;
use Zephir\StatementsBlock;
use Zephir\Optimizers\EvalExpression;
use Zephir\Branch;

/**
 * SwitchStatement
 *
 * Switch statement, the same as in PHP/C
 */
class SwitchStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     */
    public function compile(CompilationContext $compilationContext)
    {
        $exprRaw = $this->_statement['expr'];

        $codePrinter = $compilationContext->codePrinter;

        $compilationContext->insideSwitch++;

        $exprEval = new Expression($exprRaw);
        $exprEval->setReadOnly(true);
        $resolvedExpr = $exprEval->compile($compilationContext);

        if (isset($this->_statement['clauses'])) {
            $evalExpr = new EvalExpression();

            $codePrinter->output('do {');
            $compilationContext->codePrinter->increaseLevel();

            if ($resolvedExpr->getType() != 'variable') {
                /**
                 * Create a temporary variable
                 */
                $tempVariable = $compilationContext->symbolTable->getTempVariable($resolvedExpr->getType(), $compilationContext);

                /**
                 * Simulate an assignment to the temporary variable
                 */
                $statement = new LetStatement(array(
                    'type' => 'let',
                    'assignments' => array(
                        array(
                            'assign-type' => 'variable',
                            'operator' => 'assign',
                            'variable' => $tempVariable->getName(),
                            'expr' => array(
                                'type' => $resolvedExpr->getType(),
                                'value' => $resolvedExpr->getCode(),
                                'file' => $exprRaw['file'],
                                'line' => $exprRaw['line'],
                                'char' => $exprRaw['char']
                            ),
                            'file' => $exprRaw['file'],
                            'line' => $exprRaw['line'],
                            'char' => $exprRaw['char']
                        )
                    )
                ));
                $statement->compile($compilationContext);
            } else {
                $tempVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $exprRaw);
            }

            $clauses = $this->normalizeClauses($this->_statement['clauses']);
            $tempLeft = array('type' => 'variable', 'value' => $tempVariable->getRealName());

            /**
             * In the first round we group case clauses that have block statements
             * with the ones that does not have one
             */
            $blocks = array();
            $exprStack = array();
            $defaultBlock = null;
            foreach ($clauses as $clause) {
                if ($clause['type'] == 'case') {
                    $expr = array(
                        'type' => 'equals',
                        'left' => $tempLeft,
                        'right' => $clause['expr']
                    );
                    if (!isset($clause['statements'])) {
                        $exprStack[] = $expr;
                    } else {
                        $exprStack[] = $expr;
                        $blocks[] = array(
                            'expr' => $exprStack,
                            'block' => $clause['statements']
                        );
                        $exprStack = array();
                    }
                } else {
                    if ($clause['type'] == 'default') {
                        $defaultBlock = $clause['statements'];
                    }
                }
            }

            /**
             * In the second round we generate the conditions with their blocks
             * grouping 'cases' without a statement block using an 'or'
             */
            foreach ($blocks as $block) {
                $expressions = $block['expr'];

                if (count($expressions) == 1) {
                    $condition = $evalExpr->optimize($expressions[0], $compilationContext);
                    $codePrinter->output('if (' . $condition . ') {');
                } else {
                    $orConditions = array();
                    foreach ($expressions as $expression) {
                        $orConditions[] = $evalExpr->optimize($expression, $compilationContext);
                    }
                    $codePrinter->output('if (' . join(' || ', $orConditions) . ') {');
                }

                if (isset($block['block'])) {
                    $st = new StatementsBlock($block['block']);
                    $branch = $st->compile($compilationContext, false, Branch::TYPE_SWITCH);
                    $branch->setRelatedStatement($this);
                }

                $codePrinter->output('}');
            }

            $compilationContext->codePrinter->decreaseLevel();

            /**
             * The default block is resolved at the end of the 'switch'
             */
            if ($defaultBlock) {
                $st = new StatementsBlock($defaultBlock);
                $st->compile($compilationContext);
            }

            $compilationContext->insideSwitch--;

            $codePrinter->output('} while(0);');
            $codePrinter->outputBlankLine();
        }
    }

    public function normalizeClauses($clauses)
    {
        foreach ($clauses as $defaultIndex => $clause) {
            if ($clause['type'] == 'default') {
                break;
            }
        }

        if ($defaultIndex === count($clauses) - 1) {
            return $clauses;
        }

        $emptyClausesBeforeDefault = array();
        for ($i = $defaultIndex - 1; $i >= 0; $i--) {
            $clause = $clauses[$i];
            if (isset($clause['statements'])) {
                break;
            }
            $emptyClausesBeforeDefault[] = $i;
        }

        foreach ($emptyClausesBeforeDefault as $i) {
            unset($clauses[$i]);
        }

        return $clauses;
    }
}
