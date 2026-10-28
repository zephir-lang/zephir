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
use Zephir\Expression\Builder\Operators\AssignVariableOperator;
use Zephir\Expression\Builder\Operators\BinaryOperator;
use Zephir\Statements\Let\ArrayIndexOperator;
use Zephir\Statements\Let\AssignmentFactory;
use Zephir\Variable\Variable;

/**
 * Let statement is used to assign variables
 */
class LetStatement extends StatementAbstract
{
    /**
     * Assignments whose target is reached through one or more index
     * expressions, `a[i] = v`, `this->p[i][] = v`, `self::p[i] = v`.
     */
    private const INDEXED_ASSIGN_TYPES = [
        'array-index',
        'array-index-append',
        'object-property-array-index',
        'object-property-array-index-append',
        'static-property-array-index',
        'static-property-array-index-append',
    ];

    /**
     * Expressions that emit no code of their own: a literal, a constant or a
     * plain variable, read where it is used.
     */
    private const IN_PLACE_EXPRESSIONS = [
        'null',
        'int',
        'integer',
        'long',
        'double',
        'bool',
        'string',
        'istring',
        'char',
        'variable',
        'constant',
        'static-constant-access',
        'empty-array',
    ];

    private const NATIVE_INDEX_TYPES = ['int', 'uint', 'long', 'ulong', 'bool', 'double', 'char', 'uchar'];

    /**
     * Assignments whose target is a property: `this->p`, `self::p`,
     * `this->{name}` and `this->{"name"}`.
     */
    private const PROPERTY_ASSIGN_TYPES = [
        'object-property',
        'static-property',
        'variable-dynamic-object-property',
        'string-dynamic-object-property',
    ];

    /**
     * The binary operator each compound assignment applies.
     */
    private const COMPOUND_OPERATORS = [
        AssignVariableOperator::OPERATOR_ADD                => BinaryOperator::OPERATOR_ADD,
        AssignVariableOperator::OPERATOR_SUB                => BinaryOperator::OPERATOR_SUB,
        AssignVariableOperator::OPERATOR_MUL                => BinaryOperator::OPERATOR_MUL,
        AssignVariableOperator::OPERATOR_DIV                => BinaryOperator::OPERATOR_DIV,
        AssignVariableOperator::OPERATOR_MOD                => BinaryOperator::OPERATOR_MOD,
        AssignVariableOperator::OPERATOR_CONCAT             => BinaryOperator::OPERATOR_CONCAT,
        AssignVariableOperator::OPERATOR_BITWISE_AND        => BinaryOperator::OPERATOR_BITWISE_AND,
        AssignVariableOperator::OPERATOR_BITWISE_OR         => BinaryOperator::OPERATOR_BITWISE_OR,
        AssignVariableOperator::OPERATOR_BITWISE_XOR        => BinaryOperator::OPERATOR_BITWISE_XOR,
        AssignVariableOperator::OPERATOR_BITWISE_SHIFTLEFT  => BinaryOperator::OPERATOR_BITWISE_SHIFT_LEFT,
        AssignVariableOperator::OPERATOR_BITWISE_SHIFTRIGHT => BinaryOperator::OPERATOR_BITWISE_SHIFT_RIGHT,
    ];

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
                 * A compound assignment no handler applies in place becomes a
                 * read, PHP's operator and a plain assignment.
                 */
                $assignment = $this->replacePropertyCompoundAssign($assignment, $compilationContext);
                $assignment = $this->replaceAssignBitwiseOnDirect($assignment);
                $assignment = $this->replaceArithmeticAssignOnDirect($assignment, $symbolVariable);
                $assignment = $this->evaluateIndexesFirst($assignment, $compilationContext);

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
     * Rewrites a compound assignment to a property, `this->p op= e`,
     * `self::p op= e`, `this->{n} op= e` or `this->{"p"} op= e`, as a read, the
     * operator and a write back, which is what PHP's ASSIGN_OBJ_OP and
     * ASSIGN_STATIC_PROP_OP do.
     *
     * PHP evaluates `e` before it reads the property, so an `e` that runs code,
     * such as a call that changes the property, is evaluated first. It also
     * converts a dynamic property name once, so a name that is not a `string`
     * is converted once into a temporary that the read and the write share.
     *
     * `self::p += e`, `self::p -= e` and `.=` on an object or static property
     * already read and write in place.
     *
     * @throws Exception
     * @throws ReflectionException
     *
     * @see https://github.com/zephir-lang/zephir/issues/2753
     */
    protected function replacePropertyCompoundAssign(array $assignment, CompilationContext $compilationContext): array
    {
        $binaryOperator = self::COMPOUND_OPERATORS[$assignment['operator'] ?? ''] ?? null;
        if (null === $binaryOperator || !in_array($assignment['assign-type'], self::PROPERTY_ASSIGN_TYPES, true)) {
            return $assignment;
        }

        $inPlace = match ($assignment['operator']) {
            AssignVariableOperator::OPERATOR_ADD,
            AssignVariableOperator::OPERATOR_SUB    => 'static-property' === $assignment['assign-type'],
            AssignVariableOperator::OPERATOR_CONCAT => in_array(
                $assignment['assign-type'],
                ['object-property', 'static-property'],
                true
            ),
            default                                 => false,
        };
        if ($inPlace) {
            return $assignment;
        }

        if (!in_array($assignment['expr']['type'], self::IN_PLACE_EXPRESSIONS, true)) {
            $assignment['expr'] = $this->evaluatedIndex($assignment['expr'], $compilationContext);
        }

        if ('variable-dynamic-object-property' === $assignment['assign-type']) {
            $assignment['property'] = $this->propertyNameOnce($assignment, $compilationContext);
        }

        $object   = $this->astNode($assignment, ['type' => 'variable', 'value' => $assignment['variable']]);
        $property = $this->astNode($assignment, [
            'type'  => 'string-dynamic-object-property' === $assignment['assign-type'] ? 'string' : 'variable',
            'value' => $assignment['property'],
        ]);
        $target   = $this->astNode($assignment, [
            'type'  => match ($assignment['assign-type']) {
                'object-property'                  => 'property-access',
                'static-property'                  => 'static-property-access',
                'variable-dynamic-object-property' => 'property-dynamic-access',
                'string-dynamic-object-property'   => 'property-string-access',
            },
            'left'  => $object,
            'right' => $property,
        ]);

        $assignment['expr']     = $this->astNode($assignment, [
            'type'                => $binaryOperator,
            'left'                => $target,
            'right'               => $assignment['expr'],
            'compound-assignment' => true,
        ]);
        $assignment['operator'] = AssignVariableOperator::OPERATOR_ASSIGN;

        return $assignment;
    }

    /**
     * The name of `this->{name}` as a string the read and the write of a
     * compound assignment share, converted once as PHP converts it.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function propertyNameOnce(array $assignment, CompilationContext $compilationContext): string
    {
        $name = $compilationContext->symbolTable->getVariableForRead(
            $assignment['property'],
            $compilationContext,
            $assignment
        );
        if ('string' === $name->getType()) {
            return $assignment['property'];
        }

        $cast = new Expression($this->astNode($assignment, [
            'type'  => 'cast',
            'left'  => 'string',
            'right' => $this->astNode($assignment, ['type' => 'variable', 'value' => $assignment['property']]),
        ]));

        return (string) $cast->compile($compilationContext)->getCode();
    }

    /**
     * Rewrites `x /= e` and `x %= e` as `x = x / e` and `x = x % e`, so
     * DivOperator and ModOperator produce the value PHP does, with its
     * TypeError and its zero divisor guard. A C `/=` or `%=` on a typed local
     * would raise SIGFPE for a zero divisor, and `%=` on a C double does not
     * compile.
     *
     * `+=`, `-=` and `*=` on a `var` or `mixed` local are rewritten the same
     * way, so a string, null, array or bool operand reaches PHP's own
     * operator: the direct forms rejected a string or bool operand and
     * ignored `null`. Typed number locals keep their native C forms. A
     * property is rewritten by replacePropertyCompoundAssign().
     *
     * @see https://github.com/zephir-lang/zephir/issues/2675
     * @see https://github.com/zephir-lang/zephir/issues/2676
     */
    protected function replaceArithmeticAssignOnDirect(array $assignment, ?Variable $symbolVariable): array
    {
        $binaryOperator = match ($assignment['operator'] ?? null) {
            AssignVariableOperator::OPERATOR_ADD => BinaryOperator::OPERATOR_ADD,
            AssignVariableOperator::OPERATOR_SUB => BinaryOperator::OPERATOR_SUB,
            AssignVariableOperator::OPERATOR_MUL => BinaryOperator::OPERATOR_MUL,
            AssignVariableOperator::OPERATOR_DIV => BinaryOperator::OPERATOR_DIV,
            AssignVariableOperator::OPERATOR_MOD => BinaryOperator::OPERATOR_MOD,
            default                              => null,
        };
        if (null === $binaryOperator) {
            return $assignment;
        }

        $isDivMod = in_array($binaryOperator, [BinaryOperator::OPERATOR_DIV, BinaryOperator::OPERATOR_MOD], true);
        $targets  = $isDivMod
            ? ['variable', 'mixed', 'int', 'uint', 'long', 'ulong', 'double']
            : ['variable', 'mixed'];
        $dividend = match ($assignment['assign-type']) {
            'variable' => in_array($symbolVariable?->getType(), $targets, true)
                ? $this->astNode($assignment, ['type' => 'variable', 'value' => $assignment['variable']])
                : null,
            default    => null,
        };

        if (null === $dividend) {
            return $assignment;
        }

        $assignment['expr']     = $this->astNode($assignment, [
            'type'  => $binaryOperator,
            'left'  => $dividend,
            'right' => $assignment['expr'],
        ]);
        $assignment['operator'] = AssignVariableOperator::OPERATOR_ASSIGN;

        return $assignment;
    }

    /**
     * Evaluates the index expressions of `x[i] = e` before `e`, as PHP does.
     *
     * PHP compiles every index operand before the right-hand side and delays
     * only the fetches themselves (`zend_delayed_compile_dim()`,
     * Zend/zend_compile.c), so in `a[k()] = v()` the key is computed first and
     * `a[this->n] = this->bump()` uses the counter before the bump. A plain
     * variable index is the exception: a CV operand is read when the element
     * is written, after the right-hand side, so it stays where it is, and so
     * do literals and constants, which nothing can change.
     *
     * Each such index is compiled here, ahead of the right-hand side, and its
     * node is replaced by one naming the value it produced, which the
     * assignment handler then reads without evaluating anything again. When
     * the right-hand side itself emits no code the order is not observable,
     * and the statement is left exactly as it was.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    protected function evaluateIndexesFirst(array $assignment, CompilationContext $compilationContext): array
    {
        if (
            !in_array($assignment['assign-type'], self::INDEXED_ASSIGN_TYPES, true)
            || in_array($assignment['expr']['type'], self::IN_PLACE_EXPRESSIONS, true)
        ) {
            return $assignment;
        }

        foreach ($assignment['index-expr'] as $position => $indexExpr) {
            if (in_array($indexExpr['type'], self::IN_PLACE_EXPRESSIONS, true)) {
                continue;
            }

            $assignment['index-expr'][$position] = $this->evaluatedIndex($indexExpr, $compilationContext);
        }

        return $assignment;
    }

    /**
     * Compiles one index, or the right-hand side of a compound assignment to a
     * property, and returns a node for its value.
     *
     * The value is owned, never borrowed: the right-hand side runs userland
     * code before the index is used, and that code may release whatever a
     * borrowed read pointed into. A native result is stored in a temp of its
     * own type, because its C expression would otherwise be evaluated only
     * where the handler emits it.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function evaluatedIndex(array $indexExpr, CompilationContext $compilationContext): array
    {
        $expression = new Expression($indexExpr);
        $expression->setReadOnly(false);
        $compiled = $expression->compile($compilationContext);
        $type     = $compiled->getType();

        if ('variable' === $type || 'array' === $type) {
            return $this->astNode($indexExpr, ['type' => 'variable', 'value' => $compiled->getCode()]);
        }

        if (in_array($type, self::NATIVE_INDEX_TYPES, true)) {
            $temp = $compilationContext->symbolTable->getTempVariableForWrite($type, $compilationContext);
            $compilationContext->codePrinter->output(
                $temp->getName() . ' = ' . ('bool' === $type ? $compiled->getBooleanCode() : $compiled->getCode()) . ';'
            );

            return $this->astNode($indexExpr, ['type' => 'variable', 'value' => $temp->getName()]);
        }

        return $this->astNode($indexExpr, ['type' => $type, 'value' => $compiled->getCode()]);
    }

    /**
     * Copies the source position of the statement onto a synthesized node, so
     * a diagnostic about it points at the original `/=` or `%=`.
     */
    private function astNode(array $assignment, array $node): array
    {
        foreach (['file', 'line', 'char'] as $position) {
            if (isset($assignment[$position])) {
                $node[$position] = $assignment[$position];
            }
        }

        return $node;
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

        /**
         * An array element is read and written in place by the kernel.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2747
         */
        if (ArrayIndexOperator::handles($assignment)) {
            return $assignment;
        }

        /**
         * So is a dynamic variable, `let {name} <<= 2`.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2710
         */
        if (in_array($assignment['assign-type'], ['dynamic-variable', 'dynamic-variable-string'], true)) {
            return $assignment;
        }

        /**
         * A property was rewritten by replacePropertyCompoundAssign().
         */
        if ('variable' !== $assignment['assign-type']) {
            throw new CompilerException(
                "Operator '" . $assignment['operator'] . "' is not supported assign-type: " . $assignment['assign-type']
            );
        }

        $assignment['expr']     = $this->astNode($assignment, [
            'type'                => $operator,
            'left'                => $this->astNode($assignment, ['type' => 'variable', 'value' => $assignment['variable']]),
            'right'               => $assignment['expr'],
            'compound-assignment' => true,
        ]);
        $assignment['operator'] = AssignVariableOperator::OPERATOR_ASSIGN;

        return $assignment;
    }
}
