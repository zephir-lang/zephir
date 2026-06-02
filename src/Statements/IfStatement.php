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
use Zephir\Variable\Variable;

use function array_merge;
use function array_unique;
use function array_values;
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
            /**
             * Narrow a variable's type inside an `if (var instanceof Class)`
             * guard so that methods which only exist on the subtype resolve,
             * instead of hard-erroring against the declared base type.
             * @see https://github.com/zephir-lang/zephir/issues/2565
             */
            $narrowing = $this->narrowInstanceOf($exprRaw, $compilationContext);

            $st     = new StatementsBlock($this->statement['statements']);
            $branch = $st->compile($compilationContext, $expr->isUnreachable(), Branch::TYPE_CONDITIONAL_TRUE);
            $branch->setRelatedStatement($this);

            if ($narrowing !== null) {
                [$narrowedVariable, $originalClassTypes] = $narrowing;
                $narrowedVariable->overrideClassTypes($originalClassTypes);
            }
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

    /**
     * Detects an `if (var instanceof Class)` guard and narrows the variable's
     * class types by adding the guarded subtype, so a method that only exists
     * on the subtype resolves while the truthy branch is compiled.
     *
     * The added subtype must be removed once the branch is compiled; the caller
     * restores the returned original class types with overrideClassTypes().
     *
     * @see https://github.com/zephir-lang/zephir/issues/2565
     *
     * @return array{0: Variable, 1: array}|null [variable, original class types]
     */
    private function narrowInstanceOf(array $exprRaw, CompilationContext $compilationContext): ?array
    {
        if (($exprRaw['type'] ?? null) !== 'instanceof') {
            return null;
        }

        $left  = $exprRaw['left'] ?? [];
        $right = $exprRaw['right'] ?? [];

        if (($left['type'] ?? null) !== 'variable' || ($right['type'] ?? null) !== 'variable') {
            return null;
        }

        $variableName = $left['value'];
        $className    = $right['value'];
        $symbolTable  = $compilationContext->symbolTable;

        /**
         * `this` and a right operand that is a runtime variable (rather than a
         * class name) are handled elsewhere and must not be narrowed.
         */
        if ($variableName === 'this' || $className === 'this' || $symbolTable->hasVariable($className)) {
            return null;
        }

        if (!$symbolTable->hasVariable($variableName)) {
            return null;
        }

        $variable = $symbolTable->getVariable($variableName);

        // Only narrow variables already known to hold an object.
        if ($variable->getType() !== 'variable' || !$variable->hasAnyDynamicType('object')) {
            return null;
        }

        $fullName = $compilationContext->getFullName($className);
        $compiler = $compilationContext->compiler;

        // Only narrow to a class/interface Zephir can resolve at compile time.
        if (
            !$compiler->isClass($fullName) &&
            !$compiler->isInterface($fullName) &&
            !$compiler->isBundledClass($fullName) &&
            !$compiler->isBundledInterface($fullName)
        ) {
            return null;
        }

        $originalClassTypes = $variable->getClassTypes();

        $variable->overrideClassTypes(
            array_values(array_unique(array_merge($originalClassTypes, [$fullName])))
        );

        return [$variable, $originalClassTypes];
    }
}
