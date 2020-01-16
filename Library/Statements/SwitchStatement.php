<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements;

use Zephir\Branch;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\Optimizers\EvalExpression;
use Zephir\StatementsBlock;

/**
 * SwitchStatement.
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
        $exprRaw = $this->statement['expr'];

        $codePrinter = $compilationContext->codePrinter;

        ++$compilationContext->insideSwitch;

        $exprEval = new Expression($exprRaw);
        $exprEval->setReadOnly(true);
        $resolvedExpr = $exprEval->compile($compilationContext);

        if (isset($this->statement['clauses'])) {
            $evalExpr = new EvalExpression();

            $codePrinter->output('do {');
            $compilationContext->codePrinter->increaseLevel();

            if ('variable' != $resolvedExpr->getType()) {
                /**
                 * Create a temporary variable.
                 */
                $tempVariable = $compilationContext->symbolTable->getTempVariable($resolvedExpr->getType(), $compilationContext);

                /**
                 * Simulate an assignment to the temporary variable.
                 */
                $statement = new LetStatement([
                    'type' => 'let',
                    'assignments' => [
                        [
                            'assign-type' => 'variable',
                            'operator' => 'assign',
                            'variable' => $tempVariable->getName(),
                            'expr' => [
                                'type' => $resolvedExpr->getType(),
                                'value' => $resolvedExpr->getCode(),
                                'file' => $exprRaw['file'],
                                'line' => $exprRaw['line'],
                                'char' => $exprRaw['char'],
                            ],
                            'file' => $exprRaw['file'],
                            'line' => $exprRaw['line'],
                            'char' => $exprRaw['char'],
                        ],
                    ],
                ]);
                $statement->compile($compilationContext);
            } else {
                $tempVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $exprRaw);
            }

            $clauses = $this->normalizeClauses($this->statement['clauses']);
            $tempLeft = ['type' => 'variable', 'value' => $tempVariable->getRealName()];

            /**
             * In the first round we group case clauses that have block statements
             * with the ones that does not have one.
             */
            $blocks = [];
            $exprStack = [];
            $defaultBlock = null;
            foreach ($clauses as $clause) {
                if ('case' == $clause['type']) {
                    $expr = [
                        'type' => 'equals',
                        'left' => $tempLeft,
                        'right' => $clause['expr'],
                    ];
                    if (!isset($clause['statements'])) {
                        $exprStack[] = $expr;
                    } else {
                        $exprStack[] = $expr;
                        $blocks[] = [
                            'expr' => $exprStack,
                            'block' => $clause['statements'],
                        ];
                        $exprStack = [];
                    }
                } else {
                    if ('default' == $clause['type']) {
                        $defaultBlock = $clause['statements'];
                    }
                }
            }

            /*
             * In the second round we generate the conditions with their blocks
             * grouping 'cases' without a statement block using an 'or'
             */
            foreach ($blocks as $block) {
                $expressions = $block['expr'];

                if (1 == \count($expressions)) {
                    $condition = $evalExpr->optimize($expressions[0], $compilationContext);
                    $codePrinter->output('if ('.$condition.') {');
                } else {
                    $orConditions = [];
                    foreach ($expressions as $expression) {
                        $orConditions[] = $evalExpr->optimize($expression, $compilationContext);
                    }
                    $codePrinter->output('if ('.implode(' || ', $orConditions).') {');
                }

                if (isset($block['block'])) {
                    $st = new StatementsBlock($block['block']);
                    $branch = $st->compile($compilationContext, false, Branch::TYPE_SWITCH);
                    $branch->setRelatedStatement($this);
                }

                $codePrinter->output('}');
            }

            $compilationContext->codePrinter->decreaseLevel();

            /*
             * The default block is resolved at the end of the 'switch'
             */
            if ($defaultBlock) {
                $st = new StatementsBlock($defaultBlock);
                $st->compile($compilationContext);
            }

            --$compilationContext->insideSwitch;

            $codePrinter->output('} while(0);');
            $codePrinter->outputBlankLine();
        }
    }

    public function normalizeClauses($clauses)
    {
        foreach ($clauses as $defaultIndex => $clause) {
            if ('default' == $clause['type']) {
                break;
            }
        }

        if ($defaultIndex === \count($clauses) - 1) {
            return $clauses;
        }

        $emptyClausesBeforeDefault = [];
        for ($i = $defaultIndex - 1; $i >= 0; --$i) {
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
