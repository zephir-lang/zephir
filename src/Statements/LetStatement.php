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
            $variable = $assignment['variable'];

            /**
             * Get the symbol from the symbol table if necessary
             */
            $symbolVariable = match ($assignment['assign-type']) {
                'static-property',
                'static-property-append',
                'static-property-array-index',
                'static-property-array-index-append',
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
