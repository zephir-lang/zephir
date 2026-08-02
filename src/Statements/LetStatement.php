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
use Zephir\CompilationContext;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Expression\Builder\Operators\AssignVariableOperator;
use Zephir\Expression\Builder\Operators\BinaryOperator;
use Zephir\Statements\Let\AssignmentFactory;

/**
 * Let statement is used to assign variables
 */
class LetStatement extends StatementAbstract
{
    /**
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $readDetector = new ReadDetector();
        foreach ($this->statement['assignments'] as $assignment) {
            /**
             * Nested object-property assignments such as `let a->b->c = expr;`
             * are represented by the parser with a `property-access` assign-type
             * whose left-hand side is itself a property-access expression.
             * Resolve the inner object into a variable and rewrite the
             * assignment as a regular object-property assignment on it.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2532
             */
            if ('property-access' === $assignment['assign-type']) {
                $assignment = $this->resolveNestedObjectProperty($assignment, $compilationContext);
            }

            /**
             * Destructuring (`let [a, b] = expr;`) writes to several targets, so
             * it carries a `variables` list instead of a single `variable`.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2496
             */
            $variable = $assignment['variable'] ?? null;

            /**
             * Get the symbol from the symbol table if necessary
             */
            $symbolVariable = match ($assignment['assign-type']) {
                'static-property',
                'static-property-append',
                'static-property-array-index',
                'static-property-array-index-append',
                'destructure',
                'dynamic-variable-string'          => null,
                'array-index',
                'variable-append',
                'object-property',
                'array-index-append',
                'string-dynamic-object-property',
                'variable-dynamic-object-property' => $compilationContext->symbolTable->getVariableForUpdate(
                    $variable,
                    $compilationContext,
                    $assignment
                ),
                default                            => $compilationContext->symbolTable->getVariableForWrite(
                    $variable,
                    $compilationContext,
                    $assignment
                ),
            };

            /**
             * Incr/Decr assignments don't require an expression
             */
            $resolvedExpr = null;
            if (isset($assignment['expr'])) {
                /**
                 * Replace on direct-assignment if this bitwise-assignment
                 * TODO: Replace on supported native bitwise-assignment
                 */
                $assignment = $this->replaceAssignBitwiseOnDirect($assignment);

                $expr = new Expression($assignment['expr']);

                if ($assignment['assign-type'] === 'variable') {
                    if (!$readDetector->detect($variable, $assignment['expr'])) {
                        if (!isset($assignment['operator']) || 'assign' === $assignment['operator']) {
                            $expr->setExpectReturn(true, $symbolVariable);
                        }
                    } else {
                        if (!isset($assignment['operator']) || 'assign' === $assignment['operator']) {
                            $expr->setExpectReturn(true);
                        }
                    }
                }

                if (in_array($assignment['expr']['type'], ['property-access', 'array-access', 'type-hint'])) {
                    $expr->setReadOnly(true);
                }

                $resolvedExpr = $expr->compile($compilationContext);
            }

            if ($symbolVariable) {
                $variable = $symbolVariable->getName();
            }

            /**
             * Process assignment using factory pattern
             */
            AssignmentFactory::process(
                $assignment,
                $symbolVariable,
                $resolvedExpr,
                $compilationContext,
                $readDetector
            );
        }
    }

    /**
     * Resolves the inner object of a nested object-property assignment
     * (e.g. `let a->b->c = expr;`) into a temporary variable and rewrites
     * the assignment as a regular object-property assignment on it.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2532
     *
     * @throws Exception
     * @throws ReflectionException
     */
    protected function resolveNestedObjectProperty(
        array $assignment,
        CompilationContext $compilationContext
    ): array {
        $tempVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify(
            'variable',
            $compilationContext
        );

        $leftExpression = new Expression($assignment['left']);
        $leftExpression->setReadOnly(false);
        $leftExpression->setExpectReturn(true, $tempVariable);
        $resolvedLeft = $leftExpression->compile($compilationContext);

        if ('variable' !== $resolvedLeft->getType()) {
            throw new CompilerException(
                'Cannot use expression: ' . $resolvedLeft->getType() . ' as an object',
                $assignment['left']
            );
        }

        $assignment['assign-type'] = 'object-property';
        $assignment['variable']    = $resolvedLeft->getCode();
        unset($assignment['left']);

        return $assignment;
    }

    /**
     * @throws CompilerException
     */
    protected function replaceAssignBitwiseOnDirect(array $assignment): array
    {
        switch ($assignment['operator']) {
            case AssignVariableOperator::OPERATOR_BITWISE_AND:
                $operator = BinaryOperator::OPERATOR_BITWISE_AND;
                break;

            case AssignVariableOperator::OPERATOR_BITWISE_OR:
                $operator = BinaryOperator::OPERATOR_BITWISE_OR;
                break;

            case AssignVariableOperator::OPERATOR_BITWISE_XOR:
                $operator = BinaryOperator::OPERATOR_BITWISE_XOR;
                break;

            case AssignVariableOperator::OPERATOR_BITWISE_SHIFTLEFT:
                $operator = BinaryOperator::OPERATOR_BITWISE_SHIFT_LEFT;
                break;

            case AssignVariableOperator::OPERATOR_BITWISE_SHIFTRIGHT:
                $operator = BinaryOperator::OPERATOR_BITWISE_SHIFT_RIGHT;
                break;

            default:
                return $assignment;
        }

        if ($assignment['assign-type'] !== 'variable') {
            throw new CompilerException(
                "Operator '" . $assignment['operator'] . "' is not supported assign-type: " . $assignment['assign-type']
            );
        }

        $builderExpr        = BuilderFactory::getInstance();
        $leftExpression     = $builderExpr->variable($assignment['variable']);
        $assignment['expr'] = $builderExpr->operators()
                                          ->binary($operator, $leftExpression, $builderExpr->raw($assignment['expr']))
                                          ->build()
        ;

        $assignment['operator'] = AssignVariableOperator::OPERATOR_ASSIGN;

        return $assignment;
    }
}
