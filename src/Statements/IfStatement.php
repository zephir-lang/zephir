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
use Zephir\Code\Printer;
use Zephir\CompilationContext;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception;
use Zephir\Optimizers\EvalExpression;
use Zephir\Passes\SkipVariantInit;
use Zephir\StatementsBlock;

use function explode;
use function is_object;

use const PHP_EOL;

/**
 * 'If' statement, the same as in PHP/C
 */
class IfStatement extends StatementAbstract
{
    /**
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $exprRaw = $this->statement['expr'];

        $expr      = new EvalExpression();
        $condition = $expr->optimize($exprRaw, $compilationContext);

        /**
         * This pass tries to move dynamic variable initialization out of the if/else branch
         */
        if (isset($this->statement['statements']) && (isset($this->statement['else_statements']) || isset($this->statement['elseif_statements']))) {
            $readDetector    = new ReadDetector();
            $skipVariantInit = new SkipVariantInit();

            $skipVariantInit->setVariablesToSkip(0, $expr->getUsedVariables());
            $skipVariantInit->pass(0, new StatementsBlock($this->statement['statements']));

            $lastBranchId = 0;

            if (isset($this->statement['else_statements'])) {
                ++$lastBranchId;
                $skipVariantInit->setVariablesToSkip($lastBranchId, $expr->getUsedVariables());
                $skipVariantInit->pass($lastBranchId, new StatementsBlock($this->statement['else_statements']));
            }

            if (isset($this->statement['elseif_statements'])) {
                foreach ($this->statement['elseif_statements'] as $key => $statement) {
                    /*
                     * Compile the elseif condition into a temporary printer so
                     * that any preamble code the expression compiler emits
                     * (e.g. zephir_array_fetch_long for myvar[0]) is captured
                     * and NOT written directly to the output before the outer
                     * if-check. The captured preamble is replayed later, inside
                     * the else-branch that guards it, so it only runs when the
                     * preceding if/elseif branches were all false.
                     */
                    $originalPrinter                     = $compilationContext->codePrinter;
                    $compilationContext->codePrinter      = new Printer();

                    $this->statement['elseif_statements'][$key]['condition'] = $expr->optimize(
                        $statement['expr'],
                        $compilationContext
                    );

                    $this->statement['elseif_statements'][$key]['condition_preamble'] =
                        $compilationContext->codePrinter->getOutput();

                    $compilationContext->codePrinter = $originalPrinter;

                    ++$lastBranchId;
                    $skipVariantInit->setVariablesToSkip($lastBranchId, $expr->getUsedVariables());

                    if (!isset($statement['statements'])) {
                        continue;
                    }

                    $skipVariantInit->pass($lastBranchId, new StatementsBlock($statement));
                }
            }

            $symbolTable = $compilationContext->symbolTable;
            foreach ($skipVariantInit->getVariables() as $variable) {
                if ($symbolTable->hasVariable((string)$variable)) {
                    $symbolVariable = $symbolTable->getVariable($variable);
                    if ('variable' === $symbolVariable->getType()) {
                        if (!$readDetector->detect($variable, $exprRaw)) {
                            $symbolVariable->initVariant($compilationContext);
                            $symbolVariable->skipInitVariant(2);
                        }
                    }
                }
            }
        }

        $compilationContext->codePrinter->output('if (' . $condition . ') {');
        $this->evalExpression = $expr;

        /**
         * Try to mark latest temporary variable used as idle.
         */
        $evalVariable = $expr->getEvalVariable();
        if (is_object($evalVariable)) {
            $this->checkVariableTemporal($evalVariable);
        }

        /**
         * Compile statements in the 'if' block
         */
        if (isset($this->statement['statements'])) {
            $st     = new StatementsBlock($this->statement['statements']);
            $branch = $st->compile($compilationContext, $expr->isUnreachable(), Branch::TYPE_CONDITIONAL_TRUE);
            $branch->setRelatedStatement($this);
        }

        /**
         * Compile statements in the 'elseif' block.
         *
         * When a condition has a non-empty preamble (side-effect code captured
         * above), it must run only when no preceding branch was taken. We wrap
         * it in an explicit else-block: "} else { preamble; if (cond) { ... }".
         * Without a preamble the original flat "} else if (cond) {" form is kept
         * so the generated C is unchanged for simple conditions.
         */
        $nestedElseCount = 0;

        if (isset($this->statement['elseif_statements'])) {
            foreach ($this->statement['elseif_statements'] as $statement) {
                if (!isset($statement['statements'])) {
                    continue;
                }

                $preamble      = $statement['condition_preamble'] ?? '';
                $elseIfCond    = $statement['condition'];

                if ($preamble !== '') {
                    $compilationContext->codePrinter->output('} else {');
                    $compilationContext->codePrinter->increaseLevel();
                    $nestedElseCount++;

                    foreach (explode(PHP_EOL, rtrim($preamble)) as $line) {
                        if ($line !== '') {
                            $compilationContext->codePrinter->output($line);
                        }
                    }

                    $compilationContext->codePrinter->output('if (' . $elseIfCond . ') {');
                } else {
                    $compilationContext->codePrinter->output('} else if (' . $elseIfCond . ') {');
                }

                $st     = new StatementsBlock($statement['statements']);
                $branch = $st->compile($compilationContext, $expr->isUnreachable(), Branch::TYPE_CONDITIONAL_TRUE);
                $branch->setRelatedStatement($this);
            }
        }

        /**
         * Compile statements in the 'else' block
         */
        if (isset($this->statement['else_statements'])) {
            $compilationContext->codePrinter->output('} else {');
            $st     = new StatementsBlock($this->statement['else_statements']);
            $branch = $st->compile($compilationContext, $expr->isUnreachableElse(), Branch::TYPE_CONDITIONAL_FALSE);
            $branch->setRelatedStatement($this);
        }

        $compilationContext->codePrinter->output('}');

        /*
         * Each preamble-wrapped elseif opened an extra "} else {" block.
         * Close them now, from innermost to outermost.
         */
        for ($i = 0; $i < $nestedElseCount; $i++) {
            $compilationContext->codePrinter->decreaseLevel();
            $compilationContext->codePrinter->output('}');
        }
    }
}
