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
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Expression;
use Zephir\Optimizers\EvalExpression;
use Zephir\StatementsBlock;
use Zephir\Variable\Variable;

use function sprintf;

/**
 * Switch statement, with the same semantics as PHP's.
 *
 * Lowered to a dispatch chain of `goto`s followed by the clause bodies in
 * source order:
 *
 *     if (COND_0) { goto zephir_switch_0_clause_0; }
 *     if (COND_1) { goto zephir_switch_0_clause_1; }
 *     goto zephir_switch_0_clause_2;      // the `default`, or the end label
 *     zephir_switch_0_clause_0: ;
 *         <body 0>
 *     zephir_switch_0_clause_1: ;
 *         <body 1>
 *     zephir_switch_0_clause_2: ;
 *         <default body>
 *     zephir_switch_0_end: ;
 *
 * Emitting the bodies one after another is what gives PHP's fall-through: a
 * clause that does not jump away runs on into the next one, and a `default`
 * written in the middle falls into the `case` after it. Conditions are tested
 * in source order and the `goto` skips the rest, so a `case` expression after
 * the first match is never evaluated - again as in PHP.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1704
 */
class SwitchStatement extends StatementAbstract
{
    /**
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $exprRaw     = $this->statement['expr'];
        $codePrinter = $compilationContext->codePrinter;

        $exprEval = new Expression($exprRaw);
        $exprEval->setReadOnly(true);
        $resolvedExpr = $exprEval->compile($compilationContext);

        $clauses = $this->statement['clauses'] ?? [];
        if ([] === $clauses) {
            /**
             * `switch expr { }` evaluates the expression and does nothing else.
             */
            return;
        }

        $labelId  = $compilationContext->switchLabelId++;
        $endLabel = sprintf('zephir_switch_%d_end', $labelId);

        $scrutinee = $this->resolveScrutinee($resolvedExpr, $exprRaw, $compilationContext);
        $left      = ['type' => 'variable', 'value' => $scrutinee->getRealName()];

        $evalExpr     = new EvalExpression();
        $defaultLabel = null;

        foreach ($clauses as $index => $clause) {
            if ('default' === $clause['type']) {
                $defaultLabel = $this->clauseLabel($labelId, $index);
                continue;
            }

            $condition = $evalExpr->optimize(
                ['type' => 'equals', 'left' => $left, 'right' => $clause['expr']],
                $compilationContext
            );

            $codePrinter->output(
                sprintf('if (%s) { goto %s; }', $condition, $this->clauseLabel($labelId, $index))
            );
        }

        /**
         * Nothing matched: enter at the `default` clause wherever it is
         * written, or leave the `switch` when there is none.
         */
        $codePrinter->output(sprintf('goto %s;', $defaultLabel ?? $endLabel));

        $compilationContext->pushSwitchTarget($endLabel);

        foreach ($clauses as $index => $clause) {
            /**
             * The trailing `;` keeps the label legal even when the clause is
             * empty and nothing follows it.
             */
            $codePrinter->output($this->clauseLabel($labelId, $index) . ': ;');

            if (empty($clause['statements'])) {
                continue;
            }

            $branch = (new StatementsBlock($clause['statements']))
                ->compile($compilationContext, false, Branch::TYPE_SWITCH);
            $branch->setRelatedStatement($this);
        }

        /**
         * The end label is emitted only when something jumps to it, so that a
         * `switch` whose every clause returns does not leave an unused label
         * behind.
         */
        if ($compilationContext->popSwitchTarget() || null === $defaultLabel) {
            $codePrinter->output($endLabel . ': ;');
        }

        $codePrinter->outputBlankLine();
    }

    /**
     * Name of the label a clause's body starts at.
     */
    private function clauseLabel(int $labelId, int $index): string
    {
        return sprintf('zephir_switch_%d_clause_%d', $labelId, $index);
    }

    /**
     * The value being switched on, held in a variable so that the dispatch
     * chain can compare against it repeatedly without re-evaluating it.
     *
     * @throws ReflectionException
     * @throws Exception
     */
    private function resolveScrutinee(
        CompiledExpression $resolvedExpr,
        array $exprRaw,
        CompilationContext $compilationContext
    ): Variable {
        if ('variable' === $resolvedExpr->getType()) {
            return $compilationContext->symbolTable->getVariableForRead(
                $resolvedExpr->getCode(),
                $compilationContext,
                $exprRaw
            );
        }

        $tempVariable = $compilationContext->symbolTable->getTempVariable(
            $resolvedExpr->getType(),
            $compilationContext
        );

        /**
         * Simulate an assignment to the temporary variable.
         */
        $statement = new LetStatement([
            'type'        => 'let',
            'assignments' => [
                [
                    'assign-type' => 'variable',
                    'operator'    => 'assign',
                    'variable'    => $tempVariable->getName(),
                    'expr'        => [
                        'type'  => $resolvedExpr->getType(),
                        'value' => $resolvedExpr->getCode(),
                        'file'  => $exprRaw['file'],
                        'line'  => $exprRaw['line'],
                        'char'  => $exprRaw['char'],
                    ],
                    'file'        => $exprRaw['file'],
                    'line'        => $exprRaw['line'],
                    'char'        => $exprRaw['char'],
                ],
            ],
        ]);
        $statement->compile($compilationContext);

        return $tempVariable;
    }
}
