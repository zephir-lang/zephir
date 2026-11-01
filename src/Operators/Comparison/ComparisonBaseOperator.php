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

namespace Zephir\Operators\Comparison;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;
use Zephir\Types\Types;

use function in_array;
use function preg_match;
use function strlen;
use function strtolower;

/**
 * This is the base operator for comparison operators
 */
class ComparisonBaseOperator extends AbstractOperator
{
    /**
     * A null literal in a comparison that comparesAsBools(). It is false, to
     * `===` too when the other operand is native: a native local cannot hold
     * null, as a null argument becomes its zero, so `int n = null` tests its
     * default with `null === n`.
     */
    private const KIND_NULL = 'null';

    private const NUMBER_TYPES = [
        Types::T_INT,
        Types::T_UINT,
        Types::T_LONG,
        Types::T_ULONG,
        Types::T_DOUBLE,
        Types::T_CHAR,
        Types::T_UCHAR,
    ];

    protected bool $commutative = false;

    /**
     * Whether this is `===` or `!==`, which also compare the types.
     */
    protected bool $strict = false;

    /**
     * Whether comparing two zvals can throw. A strict comparison never runs
     * user code; a loose one can call `__toString()`.
     */
    protected bool $zvalComparisonCanThrow = true;

    /**
     * Compile the expression.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $conditions = $this->optimizeTypeOf($expression, $compilationContext);
        if (null !== $conditions) {
            return $conditions;
        }

        $this->checkLeft($expression, CompilerException::class, $expression);
        $this->checkRight($expression, CompilerException::class, $expression);

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly(true);
        $right = $rightExpr->compile($compilationContext);

        if ($this->comparesAsBools($left, $right, $compilationContext)) {
            return $this->compileBoolComparison($left, $right, $expression, $compilationContext);
        }

        if ($this->strict && $this->mixesIntegerAndDouble($left, $right, $compilationContext)) {
            return $this->compileBoxedStrictComparison($left, $right, $expression, $compilationContext);
        }

        if ($this->comparesWithNull($left, $right)) {
            return $this->compileNullComparison($left, $right, $expression, $compilationContext);
        }

        switch ($left->getType()) {
            case 'null':
                // Only null against null is left: comparesWithNull() took the rest.
                return new CompiledExpression('bool', '(0 ' . $this->operator . ' 0)', $expression);


            case 'char':
            case 'uchar':
                // Quote the bare character, then compare it as a number.
                $left = $this->quoteCharLiteral($left, $expression['left']);
                // no break

            case 'int':
            case 'uint':
            case 'long':
            case 'double':
            case 'ulong':
                switch ($right->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'double':
                        return new CompiledExpression(
                            'bool',
                            $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode(),
                            $expression
                        );

                    case 'char':
                    case 'uchar':
                        return new CompiledExpression(
                            'bool',
                            $left->getCode() . ' ' . $this->operator . ' ' . $right->getCharCode(),
                            $expression
                        );

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                            $right->getCode(),
                            $compilationContext,
                            $expression['left']
                        );
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');

                                return new CompiledExpression(
                                    'bool',
                                    $left->getCode() . ' ' . $this->operator . ' ' . $variableRight->getName(),
                                    $expression
                                );

                            case 'variable':
                            case 'mixed':
                                $compilationContext->headersManager->add('kernel/operators');
                                $variableCode = $compilationContext->backend->getVariableCode($variableRight);
                                $zvalOperator = 'double' === $left->getType()
                                    ? $this->zvalDoubleNegOperator
                                    : $this->zvalLongNegOperator;

                                return new CompiledExpression(
                                    'bool',
                                    $zvalOperator . '(' . $variableCode . ', ' . $left->getCode() . ')',
                                    $expression
                                );

                            default:
                                throw new CompilerException(
                                    'Unknown type: ' . $variableRight->getType(),
                                    $expression['right']
                                );
                        }


                    default:
                        throw new CompilerException(
                            'Cannot compare ' . $left->getType() . ' with ' . $right->getType(),
                            $expression
                        );
                }


            case 'string':
                $variableLeft     = $compilationContext->symbolTable->getTempLocalVariableForWrite(
                    'variable',
                    $compilationContext
                );
                $variableLeftCode = $compilationContext->backend->getVariableCode($variableLeft);
                $compilationContext->backend->assignString(
                    $variableLeft,
                    $left->getCode(),
                    $compilationContext,
                    true
                );
                switch ($right->getType()) {
                    case 'string':
                        $compilationContext->headersManager->add('kernel/operators');

                        return new CompiledExpression(
                            'bool',
                            $this->zvalStringOperator . '(' . $variableLeftCode . ', "' . $right->getCode() . '")',
                            $expression['left']
                        );


                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                            $right->getCode(),
                            $compilationContext,
                            $expression['left']
                        );
                        switch ($variableRight->getType()) {
                            case 'string':
                            case 'variable':
                            case 'mixed':
                                $compilationContext->headersManager->add('kernel/operators');
                                $variableRight = $compilationContext->backend->getVariableCode($variableRight);

                                return $this->zvalComparison($variableLeftCode, $variableRight, $expression, $compilationContext);


                            default:
                                throw new CompilerException(
                                    'Unknown type: ' . $variableRight->getType(),
                                    $expression['right']
                                );
                        }


                    default:
                        throw new CompilerException('Unknown type: ' . $right->getType(), $expression['left']);
                }


            case 'variable':
                $variable     = $compilationContext->symbolTable->getVariableForRead(
                    $left->getCode(),
                    $compilationContext,
                    $expression['left']
                );
                $variableCode = $compilationContext->backend->getVariableCode($variable);
                switch ($variable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'char':
                    case 'uchar':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                return new CompiledExpression(
                                    'bool',
                                    $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode(),
                                    $expression
                                );

                            case 'char':
                            case 'uchar':
                                return new CompiledExpression(
                                    'bool',
                                    $left->getCode() . ' ' . $this->operator . ' ' . $right->getCharCode(),
                                    $expression
                                );

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['left']
                                );
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'char':
                                    case 'uchar':
                                    case 'double':
                                        return new CompiledExpression(
                                            'bool',
                                            $variable->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName(),
                                            $expression
                                        );

                                    case 'variable':
                                    case 'mixed':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRightCode = $compilationContext->backend->getVariableCode(
                                            $variableRight
                                        );

                                        // A native local is read by name; getVariableCode() takes the address of most.
                                        return new CompiledExpression(
                                            'bool',
                                            $this->zvalLongNegOperator . '(' . $variableRightCode . ', ' . $variable->getName() . ')',
                                            $expression
                                        );


                                    default:
                                        throw new CompilerException(
                                            'Unknown type: ' . $variableRight->getType(),
                                            $expression['right']
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    'Cannot compare variable: ' . $variable->getType() . ' with: ' . $right->getType(),
                                    $expression
                                );
                        }


                    case 'double':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                return new CompiledExpression(
                                    'bool',
                                    $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode(),
                                    $expression
                                );

                            case 'char':
                            case 'uchar':
                                return new CompiledExpression(
                                    'bool',
                                    $left->getCode() . ' ' . $this->operator . ' ' . $right->getCharCode(),
                                    $expression
                                );

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['left']
                                );
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'double':
                                        return new CompiledExpression(
                                            'bool',
                                            $variable->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName(),
                                            $expression
                                        );

                                    case 'variable':
                                    case 'mixed':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRightCode = $compilationContext->backend->getVariableCode(
                                            $variableRight
                                        );

                                        return new CompiledExpression(
                                            'bool',
                                            $this->zvalDoubleNegOperator . '(' . $variableRightCode . ', ' . $variable->getName() . ')',
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            'Unknown type: ' . $variableRight->getType(),
                                            $expression['right']
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    'Cannot compare variable: ' . $variable->getType() . ' with: ' . $right->getType(),
                                    $expression
                                );
                        }


                    case 'array':
                        switch ($right->getType()) {
                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['left']
                                );
                                switch ($variableRight->getType()) {
                                    case 'string':
                                    case 'variable':
                                    case 'mixed':
                                    case 'array':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRight = $compilationContext->backend->getVariableCode($variableRight);

                                        return $this->zvalComparison($variableCode, $variableRight, $expression, $compilationContext);

                                    default:
                                        throw new CompilerException(
                                            'Unknown type: ' . $variableRight->getType(),
                                            $expression['right']
                                        );
                                }


                            default:
                                throw new CompilerException('Unknown type: ' . $right->getType(), $expression['left']);
                        }


                    case 'string':
                        $compilationContext->headersManager->add('kernel/operators');

                        switch ($right->getType()) {
                            case 'string':
                                return new CompiledExpression(
                                    'bool',
                                    $this->zvalStringOperator . '(' . $variableCode . ', "' . $right->getCode() . '")',
                                    $expression['left']
                                );

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['left']
                                );

                                switch ($variableRight->getType()) {
                                    case 'string':
                                    case 'variable':
                                    case 'mixed':
                                        $variableRight = $compilationContext->backend->getVariableCode($variableRight);

                                        return $this->zvalComparison($variableCode, $variableRight, $expression, $compilationContext);

                                    default:
                                        throw new CompilerException(
                                            'Unknown type: ' . $variableRight->getType(),
                                            $expression['right']
                                        );
                                }


                            default:
                                throw new CompilerException('Unknown type: ' . $right->getType(), $expression['left']);
                        }


                    case 'variable':
                    case 'mixed':
                        $compilationContext->headersManager->add('kernel/operators');

                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression(
                                    'bool',
                                    $this->zvalLongOperator . '(' . $variableCode . ', ' . $right->getCode() . ')',
                                    $expression['left']
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'bool',
                                    $this->zvalDoubleOperator . '(' . $variableCode . ', ' . $right->getCode() . ')',
                                    $expression['left']
                                );

                            case 'char':
                            case 'uchar':
                                return new CompiledExpression(
                                    'bool',
                                    $this->zvalLongOperator . '(' . $variableCode . ', ' . $right->getCharCode() . ')',
                                    $expression['left']
                                );

                            case 'string':
                                return new CompiledExpression(
                                    'bool',
                                    $this->zvalStringOperator . '(' . $variableCode . ', "' . $right->getCode() . '")',
                                    $expression['left']
                                );

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['left']
                                );
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression(
                                            'bool',
                                            $this->zvalLongOperator . '(' . $variableCode . ', ' . $variableRight->getName(
                                            ) . ')',
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'bool',
                                            $this->zvalDoubleOperator . '(' . $variableCode . ', ' . $variableRight->getName(
                                            ) . ')',
                                            $expression
                                        );

                                    case 'string':
                                    case 'variable':
                                    case 'mixed':
                                    case 'array':
                                        $variableRight = $compilationContext->backend->getVariableCode($variableRight);

                                        return $this->zvalComparison($variableCode, $variableRight, $expression, $compilationContext);

                                    default:
                                        throw new CompilerException(
                                            'Unknown type: ' . $variableRight->getType(),
                                            $expression['right']
                                        );
                                }


                            default:
                                throw new CompilerException('Unknown type: ' . $right->getType(), $expression['left']);
                        }


                    default:
                        throw new CompilerException('Unknown type: ' . $variable->getType(), $expression);
                }


            default:
                throw new CompilerException('Unknown type: ' . $left->getType(), $expression);
        }
    }

    /**
     * @param array              $expr
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression|null
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function optimizeTypeOf(array $expr, CompilationContext $compilationContext): ?CompiledExpression
    {
        if (!isset($expr['left'])) {
            return null;
        }

        if (!isset($expr['right']) && !isset($expr['right']['value'])) {
            return null;
        }

        if ('typeof' !== $expr['left']['type']) {
            return null;
        }

        if ('string' !== $expr['right']['type']) {
            $compilationContext->logger->warning(
                "Possible invalid comparison for 'typeof' operator with non-string",
                ['invalid-typeof-comparison', $expr['right']]
            );

            return null;
        }

        switch ($expr['type']) {
            case 'identical':
            case 'equals':
                $operator = '==';
                break;

            case 'not-identical':
            case 'not-equals':
                $operator = '!=';
                break;

            default:
                return null;
        }

        $code             = (new Expression($expr['left']['left']))->compile($compilationContext)->getCode();
        $variableVariable = $compilationContext->symbolTable->getVariableForRead($code, $compilationContext, $expr);

        if ('string' !== $expr['right']['type']) {
            throw new CompilerException('Right expression of typeof operator must be "string" type', $expr['right']);
        }

        $value = strtolower($expr['right']['value']);

        switch ($variableVariable->getType()) {
            case 'double':
                $condition = match ($value) {
                    'double', 'float' => '1 ' . $operator . ' 1',
                    default           => '1 ' . $operator . ' 0',
                };
                break;

            case 'int':
            case 'integer':
            case 'long':
                $condition = match ($value) {
                    'int', 'integer', 'long' => '1 ' . $operator . ' 1',
                    default                  => '1 ' . $operator . ' 0',
                };
                break;

            case 'bool':
                $condition = match ($value) {
                    'bool', 'boolean' => '1 ' . $operator . ' 1',
                    default           => '1 ' . $operator . ' 0',
                };
                break;

            case 'array':
                $condition = match ($value) {
                    'array' => '1 ' . $operator . ' 1',
                    default => '1 ' . $operator . ' 0',
                };
                break;

            case 'string':
                $condition = match ($value) {
                    'string' => '1 ' . $operator . ' 1',
                    default  => '1 ' . $operator . ' 0',
                };
                break;

            case 'variable':
                $condition = $compilationContext->backend->getTypeofCondition($variableVariable, $operator, $value);
                break;

            default:
                return null;
        }

        return new CompiledExpression('bool', $condition, $expr);
    }

    /**
     * PHP compares a bool with any operand, and null with a number, as two
     * bools (zend_compare()): the other operand is converted to bool, and
     * `===` holds only between two equal bools. The typed branches of
     * compile() would compare such operands as numbers, or not at all (#2771).
     */
    private function comparesAsBools(
        CompiledExpression $left,
        CompiledExpression $right,
        CompilationContext $compilationContext
    ): bool {
        $leftType  = $this->operandType($left, $compilationContext);
        $rightType = $this->operandType($right, $compilationContext);

        if (Types::T_BOOL === $leftType || Types::T_BOOL === $rightType) {
            return true;
        }

        return (Types::T_NULL === $leftType && in_array($rightType, self::NUMBER_TYPES, true))
            || (Types::T_NULL === $rightType && in_array($leftType, self::NUMBER_TYPES, true));
    }

    /**
     * Whether one operand is a native float and the other a native integer,
     * which `===` never finds identical, while C compares them as numbers.
     */
    private function mixesIntegerAndDouble(
        CompiledExpression $left,
        CompiledExpression $right,
        CompilationContext $compilationContext
    ): bool {
        $types = [
            $this->operandType($left, $compilationContext),
            $this->operandType($right, $compilationContext),
        ];
        if (!in_array(Types::T_DOUBLE, $types, true)) {
            return false;
        }

        $other = Types::T_DOUBLE === $types[0] ? $types[1] : $types[0];

        return Types::T_DOUBLE !== $other && in_array($other, self::NUMBER_TYPES, true);
    }

    /**
     * Boxes both native operands and lets PHP's `===` decide, so the result
     * is not a constant the branch optimizer would fold.
     *
     * @throws CompilerException
     */
    private function compileBoxedStrictComparison(
        CompiledExpression $left,
        CompiledExpression $right,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $compilationContext->headersManager->add('kernel/operators');
        $leftOperand  = $this->operand($left, $expression, $compilationContext);
        $rightOperand = $this->operand($right, $expression, $compilationContext);

        return $this->zvalComparison(
            $this->boxedOperand($leftOperand, $compilationContext)['code'],
            $this->boxedOperand($rightOperand, $compilationContext)['code'],
            $expression,
            $compilationContext
        );
    }

    /**
     * One operand is the null literal and the other is not. Against a native
     * operand comparesAsBools() has already decided, so the other one is a
     * zval or a string or array literal.
     */
    private function comparesWithNull(CompiledExpression $left, CompiledExpression $right): bool
    {
        return (Types::T_NULL === $left->getType()) !== (Types::T_NULL === $right->getType());
    }

    /**
     * `===` and `!==` test the type. `==` and `!=` are PHP's loose comparison,
     * under which false, 0, 0.0, "" and [] equal null too, and the ordering
     * operators run PHP's own comparison (#2820). `==` used to be the type
     * test, so a variable compared that way is reported.
     *
     * @throws CompilerException
     */
    private function compileNullComparison(
        CompiledExpression $left,
        CompiledExpression $right,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $compilationContext->headersManager->add('kernel/operators');
        $nullOnLeft = Types::T_NULL === $left->getType();
        $other      = $nullOnLeft ? $right : $left;
        $code       = $this->operand($other, $expression, $compilationContext)['code'];

        if ($this->strict) {
            return new CompiledExpression('bool', 'Z_TYPE_P(' . $code . ') ' . $this->operator . ' IS_NULL', $expression);
        }

        if ('==' === $this->operator || '!=' === $this->operator) {
            if (Types::T_VARIABLE === $other->getType()) {
                $compilationContext->logger->warning(
                    sprintf(
                        "'%s' with null is PHP's loose comparison, where false, 0, 0.0, \"\" and [] equal null; "
                        . "use '%s=' to test for null",
                        $this->operator,
                        $this->operator
                    ),
                    ['loose-null-comparison', $expression]
                );
            }

            return new CompiledExpression('bool', $this->zvalNullOperator . '(' . $code . ')', $expression);
        }

        $nullCode = $this->boxedComparisonOperand(['kind' => self::KIND_NULL], $compilationContext);

        return $this->zvalOrdering(
            $nullOnLeft ? $nullCode : $code,
            $nullOnLeft ? $code : $nullCode,
            $expression,
            $compilationContext
        );
    }

    /**
     * The type of an operand, or of the variable it names, without reading it.
     */
    private function operandType(CompiledExpression $operand, CompilationContext $compilationContext): string
    {
        $symbolTable = $compilationContext->symbolTable;
        if (Types::T_VARIABLE === $operand->getType() && $symbolTable->hasVariable($operand->getCode())) {
            return $symbolTable->getVariable($operand->getCode())->getType();
        }

        return $operand->getType();
    }

    /**
     * Compiles a comparison for which comparesAsBools(). Native operands
     * compare as C bools. A zval keeps PHP's own comparison, since how an
     * object compares with a bool changed in PHP 8.5.
     *
     * @throws CompilerException
     */
    private function compileBoolComparison(
        CompiledExpression $left,
        CompiledExpression $right,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $leftOperand  = $this->boolComparisonOperand($left, $expression, $compilationContext);
        $rightOperand = $this->boolComparisonOperand($right, $expression, $compilationContext);

        if ($this->strict) {
            return $this->strictBoolComparison($leftOperand, $rightOperand, $expression, $compilationContext);
        }

        if (self::KIND_ZVAL === $leftOperand['kind'] || self::KIND_ZVAL === $rightOperand['kind']) {
            return $this->looseZvalBoolComparison($leftOperand, $rightOperand, $expression, $compilationContext);
        }

        return new CompiledExpression(
            'bool',
            $this->nativeBoolCode($leftOperand) . ' ' . $this->operator . ' ' . $this->nativeBoolCode($rightOperand),
            $expression
        );
    }

    /**
     * A null literal is kept apart from the classified operands, which
     * would box it.
     *
     * @throws CompilerException
     */
    private function boolComparisonOperand(
        CompiledExpression $operand,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        if (Types::T_NULL === $operand->getType()) {
            return ['kind' => self::KIND_NULL, 'code' => '0', 'variable' => null];
        }

        return $this->operand($operand, $expression, $compilationContext);
    }

    /**
     * The C bool, 0 or 1, that PHP converts a native operand to.
     */
    private function nativeBoolCode(array $operand): string
    {
        if (isset($operand['bool'])) {
            return $this->parenthesized($operand['bool']);
        }

        if (self::KIND_NULL === $operand['kind']) {
            return '0';
        }

        return '(' . $operand['code'] . ' != 0)';
    }

    /**
     * A bool's C code can be a condition such as `Z_TYPE_P(x) == IS_STRING`,
     * which must not merge with the comparison around it.
     */
    private function parenthesized(string $code): string
    {
        return $this->isAtomic($code) ? $code : '(' . $code . ')';
    }

    /**
     * A name, a number, a call such as `zephir_is_true(&x)` or a code
     * already wrapped in one pair of parentheses: `(a) == (b)` is not one.
     */
    private function isAtomic(string $code): bool
    {
        if ($this->isPlainName($code)) {
            return true;
        }

        if (1 !== preg_match('/^[\w$]*\(/', $code, $match)) {
            return false;
        }

        $depth  = 0;
        $length = strlen($code);
        for ($i = strlen($match[0]) - 1; $i < $length; $i++) {
            if ('(' === $code[$i]) {
                $depth++;
            } elseif (')' === $code[$i] && 0 === --$depth) {
                return $i === $length - 1;
            }
        }

        return false;
    }

    /**
     * A C identifier or number, which needs no parentheses and has no side effects.
     */
    private function isPlainName(string $code): bool
    {
        return 1 === preg_match('/^[\w$]+$/', $code);
    }

    /**
     * A zval against a bool: `==` and `!=` read the zval as a bool the way
     * PHP does; the ordering operators run PHP's own comparison.
     */
    private function looseZvalBoolComparison(
        array $leftOperand,
        array $rightOperand,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $compilationContext->headersManager->add('kernel/operators');
        $zvalOnLeft = self::KIND_ZVAL === $leftOperand['kind'];
        $zvalCode   = $zvalOnLeft ? $leftOperand['code'] : $rightOperand['code'];
        $boolCode   = $zvalOnLeft ? $rightOperand['bool'] : $leftOperand['bool'];

        if ('==' === $this->operator || '!=' === $this->operator) {
            $condition = match ($boolCode) {
                '1'     => $this->zvalBoolTrueOperator . '(' . $zvalCode . ')',
                '0'     => $this->zvalBoolFalseOperator . '(' . $zvalCode . ')',
                default => $this->zvalBoolOperator . '(' . $zvalCode . ', ' . $boolCode . ')',
            };

            return new CompiledExpression('bool', $condition, $expression);
        }

        $boxedCode = $this->boxedBool($boolCode, $compilationContext)['code'];

        return $this->zvalOrdering(
            $zvalOnLeft ? $zvalCode : $boxedCode,
            $zvalOnLeft ? $boxedCode : $zvalCode,
            $expression,
            $compilationContext
        );
    }

    /**
     * PHP compiles `a > b` as `b < a` and `a >= b` as `b <= a`, which is not
     * `!(a <= b)` for operands that do not compare, such as an enum.
     */
    private function zvalOrdering(
        string $leftCode,
        string $rightCode,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $call = match ($this->operator) {
            '<'  => 'ZEPHIR_LT(' . $leftCode . ', ' . $rightCode . ')',
            '<=' => 'ZEPHIR_LE(' . $leftCode . ', ' . $rightCode . ')',
            '>'  => 'ZEPHIR_LT(' . $rightCode . ', ' . $leftCode . ')',
            '>=' => 'ZEPHIR_LE(' . $rightCode . ', ' . $leftCode . ')',
        };

        return $this->zvalComparisonResult($call, $expression, $compilationContext);
    }

    /**
     * `===` between two bools, or between null and a native operand (see
     * KIND_NULL), compares them in C. Any other pair either holds a type
     * other than bool on one side, or a zval whose type is only known at run
     * time, so PHP's own `===` decides; a bool that is C code is boxed first,
     * so it is evaluated whatever the zval holds.
     */
    private function strictBoolComparison(
        array $leftOperand,
        array $rightOperand,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $nullAndNative = (self::KIND_NULL === $leftOperand['kind'] || self::KIND_NULL === $rightOperand['kind'])
            && self::KIND_ZVAL !== $leftOperand['kind']
            && self::KIND_ZVAL !== $rightOperand['kind'];
        if ($nullAndNative || isset($leftOperand['bool'], $rightOperand['bool'])) {
            return new CompiledExpression(
                'bool',
                $this->nativeBoolCode($leftOperand) . ' ' . $this->operator . ' ' . $this->nativeBoolCode($rightOperand),
                $expression
            );
        }

        $compilationContext->headersManager->add('kernel/operators');
        $zvalOnLeft  = self::KIND_ZVAL === $leftOperand['kind'];
        $zvalOnRight = self::KIND_ZVAL === $rightOperand['kind'];
        $boolCode    = $zvalOnLeft ? ($rightOperand['bool'] ?? null) : ($leftOperand['bool'] ?? null);
        if (($zvalOnLeft || $zvalOnRight) && null !== $boolCode) {
            $zvalCode  = $zvalOnLeft ? $leftOperand['code'] : $rightOperand['code'];
            $condition = match (true) {
                '1' === $boolCode => $this->zvalBoolTrueOperator . '(' . $zvalCode . ')',
                '0' === $boolCode => $this->zvalBoolFalseOperator . '(' . $zvalCode . ')',
                // The macro skips its bool when the zval is not one: fine for a bare local only.
                $this->isPlainName($boolCode) => $this->zvalBoolOperator . '(' . $zvalCode . ', ' . $boolCode . ')',
                default => null,
            };
            if (null !== $condition) {
                return new CompiledExpression('bool', $condition, $expression);
            }
        }

        return $this->zvalComparison(
            $this->boxedComparisonOperand($leftOperand, $compilationContext),
            $this->boxedComparisonOperand($rightOperand, $compilationContext),
            $expression,
            $compilationContext
        );
    }

    private function boxedComparisonOperand(array $operand, CompilationContext $compilationContext): string
    {
        if (self::KIND_NULL !== $operand['kind']) {
            return $this->boxedOperand($operand, $compilationContext)['code'];
        }

        $boxed = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext);
        $compilationContext->backend->assignNull($boxed, $compilationContext);

        return $compilationContext->backend->getVariableCode($boxed);
    }

    /**
     * A loose comparison of two zvals can run user code, such as a throwing
     * `__toString()`, so its result goes through a temporary and the throw
     * stops the method there, as in PHP.
     */
    protected function zvalComparison(
        string $leftCode,
        string $rightCode,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        return $this->zvalComparisonResult(
            $this->zvalOperator . '(' . $leftCode . ', ' . $rightCode . ')',
            $expression,
            $compilationContext
        );
    }

    /**
     * The result of a kernel call that compares zvals, stored in a temporary
     * with an exception check when the comparison can throw.
     */
    private function zvalComparisonResult(
        string $call,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        if (!$this->zvalComparisonCanThrow) {
            return new CompiledExpression('bool', $call, $expression);
        }

        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('bool', $compilationContext);
        $compilationContext->codePrinter->output($tempVariable->getName() . ' = ' . $call . ';');
        $compilationContext->emitExceptionCheck();

        return new CompiledExpression('bool', $tempVariable->getName(), $expression);
    }
}
