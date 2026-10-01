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

namespace Zephir\Operators\Arithmetical;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception as ZephirException;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

use function in_array;
use function sprintf;

/**
 * This is the base operator for commutative, associative and distributive
 * arithmetic operators
 */
class ArithmeticalBaseOperator extends AbstractOperator
{
    /**
     * The C kind an operand is read as by the operators that classify their
     * operands (`/` and `%`).
     */
    protected const KIND_LONG   = 'long';
    protected const KIND_DOUBLE = 'double';
    protected const KIND_ZVAL   = 'zval';

    protected bool $literalOnly = true;

    /**
     * Compiles the arithmetical operation.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws Exception
     * @throws CompilerException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $this->checkLeft($expression, CompilerException::class, $expression);
        $this->checkRight($expression, CompilerException::class, $expression);

        // Check for constant folding optimizations
        if ($optimized = $this->optimizeConstantFolding($expression, $compilationContext)) {
            return $optimized;
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly(true);
        $right = $rightExpr->compile($compilationContext);

        $zvalAware = $this->compileZvalWithScalar($left, $right, $expression, $compilationContext);
        if (null !== $zvalAware) {
            return $zvalAware;
        }

        switch ($left->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($right->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        return new CompiledExpression(
                            'int',
                            '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode() . ')',
                            $expression
                        );

                    case 'double':
                        return new CompiledExpression(
                            'double',
                            '((double) ' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode() . ')',
                            $expression
                        );

                    case 'bool':
                        return new CompiledExpression(
                            'int',
                            '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getBooleanCode() . ')',
                            $expression
                        );

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                            $right->getCode(),
                            $compilationContext,
                            $expression
                        );
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'bool':
                                return new CompiledExpression(
                                    'int',
                                    '(' . $left->getCode() . ' ' . $this->operator . ' ' . $variableRight->getName(
                                    ) . ')',
                                    $expression
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'double',
                                    '(double) (' . $left->getCode(
                                    ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                    $expression
                                );

                            default:
                                throw new CompilerException(
                                    "Cannot operate variable('int') with variable('" . $variableRight->getType() . "')",
                                    $expression
                                );
                        }

                    default:
                        throw new CompilerException(
                            "Cannot operate 'int' with '" . $right->getType() . "'",
                            $expression
                        );
                }

            case 'bool':
                return match ($right->getType()) {
                    Types::T_INT,
                    Types::T_UINT,
                    Types::T_LONG,
                    Types::T_ULONG,
                    Types::T_DOUBLE => new CompiledExpression(
                        'long',
                        '(' . $left->getBooleanCode() . ' + ' . $right->getCode() . ')',
                        $expression
                    ),
                    Types::T_BOOL   => new CompiledExpression(
                        'bool',
                        '(' . $left->getBooleanCode() . ' ' . $this->bitOperator . ' ' . $right->getBooleanCode() . ')',
                        $expression
                    ),
                    default         => throw new CompilerException(
                        "Cannot operate 'bool' with '" . $right->getType() . "'",
                        $expression
                    ),
                };

            case 'double':
                switch ($right->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        return new CompiledExpression(
                            'double',
                            '(' . $left->getCode() . ' ' . $this->operator . ' (double) (' . $right->getCode() . '))',
                            $expression
                        );

                    case 'double':
                        return new CompiledExpression(
                            'double',
                            '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode() . ')',
                            $expression
                        );

                    case 'bool':
                        return new CompiledExpression(
                            'double',
                            '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getBooleanCode() . ')',
                            $expression
                        );

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                            $right->getCode(),
                            $compilationContext,
                            $expression
                        );
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'bool':
                                return new CompiledExpression(
                                    'double',
                                    '(' . $left->getCode() . ' ' . $this->operator . ' ' . $variableRight->getName(
                                    ) . ')',
                                    $expression
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'double',
                                    '(double) (' . $left->getCode(
                                    ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                    $expression
                                );

                            default:
                                throw new CompilerException(
                                    "Cannot operate variable('double') with variable('" . $variableRight->getType(
                                    ) . "')",
                                    $expression
                                );
                        }


                    default:
                        throw new CompilerException(
                            "Cannot operate 'double' with '" . $right->getType() . "'",
                            $expression
                        );
                }


            case 'string':
                throw match ($right->getType()) {
                    default => new CompilerException(
                        'Operation is not supported between strings',
                        $expression
                    ),
                };

            case 'variable':
                $variableLeft = $compilationContext->symbolTable->getVariableForRead(
                    $left->resolve(null, $compilationContext),
                    $compilationContext,
                    $expression
                );
                switch ($variableLeft->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                return new CompiledExpression(
                                    'int',
                                    '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode() . ')',
                                    $expression
                                );

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['right']
                                );
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'bool':
                                        return new CompiledExpression(
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'double',
                                            '((double) ' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            "Cannot operate variable('int') with variable('" . $variableRight->getType(
                                            ) . "')",
                                            $expression
                                        );
                                }

                            default:
                                throw new CompilerException(
                                    "Cannot operate variable('int') with '" . $right->getType() . "'",
                                    $expression
                                );
                        }

                    case 'char':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression(
                                    'int',
                                    '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode() . ')',
                                    $expression
                                );

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['right']
                                );
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression(
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            "Cannot operate variable('char') with variable('" . $variableRight->getType(
                                            ) . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot operate variable('char') with '" . $right->getType() . "'",
                                    $expression
                                );
                        }


                    case 'bool':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode() . ')',
                                    $expression
                                );

                            case 'bool':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getCode() . ' ' . $this->bitOperator . ' ' . $right->getBooleanCode(
                                    ) . ')',
                                    $expression
                                );

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['right']
                                );
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'double':
                                        return new CompiledExpression(
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'bool':
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->bitOperator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            "Cannot operate variable('int') with variable('" . $variableRight->getType(
                                            ) . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot operate variable('int') with '" . $right->getType() . "'",
                                    $expression
                                );
                        }


                    case 'double':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression(
                                    'double',
                                    '(' . $left->getCode() . ' ' . $this->operator . ' (double) ' . $right->getCode(
                                    ) . ')',
                                    $expression
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'double',
                                    '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode() . ')',
                                    $expression
                                );

                            case 'bool':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getCode() . ' ' . $this->bitOperator . ' ' . $right->getBooleanCode(
                                    ) . ')',
                                    $expression
                                );

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['right']
                                );
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression(
                                            'double',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . '  (double) ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'double',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'bool':
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->bitOperator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            "Cannot operate variable('double') with variable('" . $variableRight->getType(
                                            ) . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot operate variable('int') with '" . $right->getType() . "'",
                                    $expression
                                );
                        }


                    case 'string':
                        throw new CompilerException("Cannot operate string variables'", $expression);

                    case 'array':
                        switch ($right->getType()) {
                            /* a(var) + a(x) */
                            case 'array':
                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->resolve(null, $compilationContext),
                                    $compilationContext,
                                    $expression
                                );
                                switch ($variableRight->getType()) {
                                    /* a(var) + a(var) */
                                    case 'array':
                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');

                                        $expected = $this->getExpected($compilationContext, $expression);
                                        $compilationContext->backend->zvalOperator(
                                            $this->zvalOperator,
                                            $expected,
                                            $variableLeft,
                                            $variableRight,
                                            $compilationContext
                                        );

                                        $this->checkVariableTemporal($variableLeft);
                                        $this->checkVariableTemporal($variableRight);

                                        $expected->setDynamicTypes(
                                            $this->getDynamicTypes($variableLeft, $variableRight)
                                        );

                                        return new CompiledExpression('variable', $expected->getName(), $expression);

                                    default:
                                        throw new CompilerException(
                                            "Cannot operate 'array with variable ('" . $variableRight->getType() . "')",
                                            $expression
                                        );
                                }
                        }
                    // no break

                    case 'variable':
                        switch ($right->getType()) {
                            /* a(var) + a(x) */
                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->resolve(null, $compilationContext),
                                    $compilationContext,
                                    $expression
                                );
                                switch ($variableRight->getType()) {
                                    /* a(var) + a(var) */
                                    case 'variable':
                                    case 'array':
                                        $compilationContext->headersManager->add('kernel/operators');

                                        $expected = $this->getExpected($compilationContext, $expression);
                                        $compilationContext->backend->zvalOperator(
                                            $this->zvalOperator,
                                            $expected,
                                            $variableLeft,
                                            $variableRight,
                                            $compilationContext
                                        );

                                        $this->checkVariableTemporal($variableLeft);
                                        $this->checkVariableTemporal($variableRight);

                                        $expected->setDynamicTypes(
                                            $this->getDynamicTypes($variableLeft, $variableRight)
                                        );

                                        return new CompiledExpression('variable', $expected->getName(), $expression);

                                    default:
                                        throw new CompilerException(
                                            "Cannot operate 'variable' with variable ('" . $variableRight->getType(
                                            ) . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot operate 'variable' with '" . $right->getType() . "'",
                                    $expression
                                );
                        }


                    default:
                        throw CompilerException::unknownType($variableLeft, $expression);
                }
            // no break

            default:
                throw CompilerException::unsupportedType($left, $expression);
        }
    }

    /**
     * This tries to perform arithmetical operations.
     *
     * Probably gcc/clang will optimize them without this optimization
     *
     * @see https://en.wikipedia.org/wiki/Constant_folding
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return bool|CompiledExpression
     */
    public function optimizeConstantFolding(array $expression, CompilationContext $compilationContext)
    {
        if ('int' != $expression['left']['type'] && 'double' != $expression['left']['type']) {
            return false;
        }

        if ($compilationContext->config->get('constant-folding', 'optimizations')) {
            if ('int' == $expression['left']['type'] && 'int' == $expression['right']['type']) {
                $left  = self::literalToNumber($expression['left']['value']);
                $right = self::literalToNumber($expression['right']['value']);
                switch ($this->operator) {
                    case '+':
                        return new CompiledExpression('int', $left + $right, $expression);

                    case '-':
                        return new CompiledExpression('int', $left - $right, $expression);

                    case '*':
                        return new CompiledExpression('int', $left * $right, $expression);
                }
            }

            if (('double' == $expression['left']['type'] && 'double' == $expression['right']['type']) || ('double' == $expression['left']['type'] && 'int' == $expression['right']['type']) || ('int' == $expression['left']['type'] && 'double' == $expression['right']['type'])) {
                $left  = self::literalToNumber($expression['left']['value']);
                $right = self::literalToNumber($expression['right']['value']);
                switch ($this->operator) {
                    case '+':
                        return new CompiledExpression('double', $left + $right, $expression);

                    case '-':
                        return new CompiledExpression('double', $left - $right, $expression);

                    case '*':
                        return new CompiledExpression('double', $left * $right, $expression);
                }
            }
        }

        return false;
    }

    /**
     * @param Variable $variable
     *
     * @return string
     */
    protected function getIsLocal(Variable $variable): string
    {
        return $variable->isLocalOnly() ? '&' : '';
    }

    /**
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return array
     * @throws ReflectionException
     * @throws ZephirException
     */
    protected function preCompileChecks(
        array $expression,
        CompilationContext $compilationContext
    ): array {
        $this->checkLeft($expression);
        $this->checkRight($expression);

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly(true);
        $right = $rightExpr->compile($compilationContext);

        $compilationContext->headersManager->add('kernel/operators');
        return [$left, $right];
    }

    /**
     * A zval combined with a native number can hold a float, a numeric
     * string or an operand PHP rejects, so reading it as a C number and
     * typing the result `int` truncates floats and never throws. The native
     * side is boxed instead and PHP's own operator computes the result, as
     * an int or a float, with PHP's TypeError and warnings. The zval's
     * dynamic types are not trusted to prove it holds an int: they grow as
     * the method compiles, so a later assignment in a loop body is missed.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2675
     * @see https://github.com/zephir-lang/zephir/issues/2744
     */
    private function compileZvalWithScalar(
        CompiledExpression $left,
        CompiledExpression $right,
        array $expression,
        CompilationContext $compilationContext
    ): ?CompiledExpression {
        $leftVariable  = $this->zvalOperand($left, $compilationContext, $expression);
        $rightVariable = $this->zvalOperand($right, $compilationContext, $expression);
        if ((null === $leftVariable) === (null === $rightVariable)) {
            return null;
        }

        $leftVariable ??= $this->boxedScalar($left, $compilationContext, $expression);
        $rightVariable ??= $this->boxedScalar($right, $compilationContext, $expression);
        if (null === $leftVariable || null === $rightVariable) {
            return null;
        }

        $compilationContext->headersManager->add('kernel/operators');

        $expected = $this->getExpected($compilationContext, $expression);
        $compilationContext->backend->zvalOperator(
            $this->zvalOperator,
            $expected,
            $leftVariable,
            $rightVariable,
            $compilationContext
        );

        $this->checkVariableTemporal($leftVariable);
        $this->checkVariableTemporal($rightVariable);

        $expected->setDynamicTypes([Types::T_LONG, Types::T_DOUBLE]);

        return new CompiledExpression('variable', $expected->getName(), $expression);
    }

    /**
     * The variable behind an operand that is a zval at runtime, or null.
     */
    private function zvalOperand(
        CompiledExpression $operand,
        CompilationContext $compilationContext,
        array $expression
    ): ?Variable {
        if ('variable' !== $operand->getType()) {
            return null;
        }

        $variable = $compilationContext->symbolTable->getVariableForRead(
            $operand->getCode(),
            $compilationContext,
            $expression
        );

        $zvalTypes = [Types::T_VARIABLE, Types::T_MIXED, Types::T_ARRAY];

        return in_array($variable->getType(), $zvalTypes, true) ? $variable : null;
    }

    /**
     * A native number, literal or typed local, copied into a temporary zval.
     * A bool stays a bool, so a TypeError names it as PHP does; a char is its
     * integer byte value.
     */
    private function boxedScalar(
        CompiledExpression $operand,
        CompilationContext $compilationContext,
        array $expression
    ): ?Variable {
        $type = $operand->getType();
        $code = Types::T_BOOL === $type ? $operand->getBooleanCode() : $operand->getCode();

        if ('variable' === $type) {
            $variable = $compilationContext->symbolTable->getVariableForRead(
                $operand->getCode(),
                $compilationContext,
                $expression
            );
            $type = $variable->getType();
            $code = $variable->getName();
        }

        $numberTypes = [
            Types::T_INT,
            Types::T_UINT,
            Types::T_LONG,
            Types::T_ULONG,
            Types::T_CHAR,
            Types::T_UCHAR,
            Types::T_DOUBLE,
            Types::T_BOOL,
        ];
        if (!in_array($type, $numberTypes, true)) {
            return null;
        }

        $backend = $compilationContext->backend;
        $boxed   = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext);
        match ($type) {
            Types::T_DOUBLE => $backend->assignDouble($boxed, $code, $compilationContext),
            Types::T_BOOL   => $backend->assignBool($boxed, $code, $compilationContext),
            default         => $backend->assignLong($boxed, $code, $compilationContext),
        };

        return $boxed;
    }

    /**
     * Returns proper dynamic types.
     *
     * @param Variable $left
     * @param Variable $right
     *
     * @return string
     */
    private function getDynamicTypes(Variable $left, Variable $right): string
    {
        if ('/' === $this->operator) {
            return Types::T_DOUBLE;
        }

        switch ($left->getType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
                switch ($right->getType()) {
                    case Types::T_INT:
                    case Types::T_UINT:
                    case Types::T_LONG:
                    case Types::T_ULONG:
                        return Types::T_INT;
                }
                break;
        }

        return Types::T_DOUBLE;
    }

    /**
     * Both operands classified as a C integer, a C double or a zval. Against
     * a zval a bool stays a bool, so a TypeError names it as PHP does.
     *
     * @return array{0: array, 1: array}
     *
     * @throws CompilerException
     */
    protected function classifiedOperands(
        CompiledExpression $left,
        CompiledExpression $right,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        $leftOperand  = $this->operand($left, $expression, $compilationContext);
        $rightOperand = $this->operand($right, $expression, $compilationContext);
        if (self::KIND_ZVAL === $leftOperand['kind'] && isset($rightOperand['bool'])) {
            $rightOperand = $this->boxedBool($rightOperand['bool'], $compilationContext);
        }
        if (self::KIND_ZVAL === $rightOperand['kind'] && isset($leftOperand['bool'])) {
            $leftOperand = $this->boxedBool($leftOperand['bool'], $compilationContext);
        }

        return [$leftOperand, $rightOperand];
    }

    /**
     * Classifies one compiled operand as a C integer, a C double or a zval,
     * and returns the C code that reads it as that kind.
     *
     * @return array{kind: string, code: string, variable: ?Variable}
     *
     * @throws CompilerException
     */
    protected function operand(
        CompiledExpression $operand,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        switch ($operand->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return ['kind' => self::KIND_LONG, 'code' => $operand->getCode(), 'variable' => null];

            case 'bool':
                return $this->boolOperand($operand->getBooleanCode());

            case 'double':
                return ['kind' => self::KIND_DOUBLE, 'code' => $operand->getCode(), 'variable' => null];

            case 'variable':
                $variable = $compilationContext->symbolTable->getVariableForRead(
                    $operand->getCode(),
                    $compilationContext,
                    $expression
                );

                return $this->variableOperand($variable, $expression, $compilationContext);

            default:
                throw new CompilerException(
                    'Cannot operate ' . $operand->getType() . ' with the ' . $this->operator . ' operator',
                    $expression
                );
        }
    }

    /**
     * @return array{kind: string, code: string, variable: ?Variable}
     *
     * @throws CompilerException
     */
    protected function variableOperand(
        Variable $variable,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        switch ($variable->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return ['kind' => self::KIND_LONG, 'code' => $variable->getName(), 'variable' => null];

            case 'bool':
                return $this->boolOperand($variable->getName());

            case 'double':
                return ['kind' => self::KIND_DOUBLE, 'code' => $variable->getName(), 'variable' => null];

            case 'variable':
            case 'mixed':
                return [
                    'kind'     => self::KIND_ZVAL,
                    'code'     => $compilationContext->backend->getVariableCode($variable),
                    'variable' => $variable,
                ];

            default:
                throw new CompilerException(
                    'Cannot operate ' . $variable->getType() . ' variables with the ' . $this->operator . ' operator',
                    $expression
                );
        }
    }

    /**
     * A bool is the integer 0 or 1; `bool` keeps its C code for boxing.
     *
     * @return array{kind: string, code: string, variable: ?Variable, bool: string}
     */
    protected function boolOperand(string $code): array
    {
        return ['kind' => self::KIND_LONG, 'code' => '(zend_long) ' . $code, 'variable' => null, 'bool' => $code];
    }

    /**
     * @return array{kind: string, code: string, variable: ?Variable}
     */
    protected function boxedBool(string $code, CompilationContext $compilationContext): array
    {
        $boxed = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext);
        $compilationContext->backend->assignBool($boxed, $code, $compilationContext);

        return [
            'kind'     => self::KIND_ZVAL,
            'code'     => $compilationContext->backend->getVariableCode($boxed),
            'variable' => $boxed,
        ];
    }

    /**
     * Writes the result of a zval-producing helper into the expected variable.
     *
     * @throws CompilerException
     */
    protected function zvalResult(
        string $helper,
        array $left,
        array $right,
        array $expression,
        CompilationContext $compilationContext,
        array $dynamicTypes
    ): CompiledExpression {
        $expected     = $this->getExpected($compilationContext, $expression);
        $expectedCode = $compilationContext->backend->getVariableCode($expected);

        $compilationContext->codePrinter->output(
            $helper . '(' . $expectedCode . ', ' . $left['code'] . ', ' . $right['code'] . ');'
        );

        foreach ([$left['variable'], $right['variable']] as $operandVariable) {
            if (null !== $operandVariable) {
                $this->checkVariableTemporal($operandVariable);
            }
        }

        $expected->setDynamicTypes($dynamicTypes);

        return new CompiledExpression('variable', $expected->getName(), $expression);
    }
}
