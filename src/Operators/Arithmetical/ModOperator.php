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

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Types\Types;

/**
 * Generates an arithmetical operation according to the operands
 */
class ModOperator extends ArithmeticalBaseOperator
{
    protected string $bitOperator  = '-';
    protected string $operator     = '%';
    protected string $zvalOperator = 'mod_function';

    /**
     * Compiles the arithmetical modulus operation.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        [$left, $right] = $this->preCompileChecks($expression, $compilationContext);

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
                        return new CompiledExpression(
                            'double',
                            'zephir_safe_mod_long_long(' . $left->getCode() . ', ' . $right->getCode() . ')',
                            $expression
                        );

                    case Types::T_DOUBLE:
                        return new CompiledExpression(
                            'double',
                            'zephir_safe_mod_long_double((double) ' . $left->getCode() . ', ' . $right->getCode() . ')',
                            $expression
                        );

                    case Types::T_BOOL:
                        return new CompiledExpression(
                            'bool',
                            '(' . $left->getCode() . ' - ' . $right->getBooleanCode() . ')',
                            $expression
                        );

                    case Types::T_VARIABLE:
                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                            $right->getCode(),
                            $compilationContext,
                            $expression
                        );
                        return match ($variableRight->getType()) {
                            Types::T_INT,
                            Types::T_UINT,
                            Types::T_LONG,
                            Types::T_ULONG,
                            Types::T_BOOL     => new CompiledExpression(
                                'double',
                                'zephir_safe_mod_long_long('
                                . $left->getCode() . ', ' . $variableRight->getName() . ')',
                                $expression
                            ),
                            Types::T_DOUBLE   => new CompiledExpression(
                                'double',
                                'zephir_safe_mod_long_double('
                                . $left->getCode() . ', ' . $variableRight->getName() . ')',
                                $expression
                            ),
                            Types::T_VARIABLE => new CompiledExpression(
                                'double',
                                'zephir_safe_mod_long_zval('
                                . $left->getCode()
                                . ', '
                                . $this->getIsLocal($variableRight)
                                . $variableRight->getName() . ')',
                                $expression
                            ),
                            default           => throw new CompilerException(
                                "Cannot operate variable('int') with variable('"
                                . $variableRight->getType() . "')",
                                $expression
                            ),
                        };


                    default:
                        throw new CompilerException(
                            "Cannot operate 'int' with '" . $right->getType() . "'",
                            $expression
                        );
                }


            case Types::T_BOOL:
                return $this->processLeftBoolean($left, $right, $expression);

            case Types::T_DOUBLE:
                switch ($right->getType()) {
                    case Types::T_INT:
                    case Types::T_UINT:
                    case Types::T_LONG:
                    case Types::T_ULONG:
                        return new CompiledExpression(
                            'double',
                            'zephir_safe_mod_double_long(' . $left->getCode() . ', (double) (' . $right->getCode(
                            ) . '))',
                            $expression
                        );

                    case Types::T_DOUBLE:
                        return new CompiledExpression(
                            'double',
                            'zephir_safe_mod_double_long(' . $left->getCode() . ', ' . $right->getCode() . ')',
                            $expression
                        );

                    case Types::T_BOOL:
                        return new CompiledExpression(
                            'double',
                            'zephir_safe_mod_double_long(' . $left->getCode() . ', ' . $right->getBooleanCode() . ')',
                            $expression
                        );

                    case Types::T_VARIABLE:
                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                            $right->getCode(),
                            $compilationContext,
                            $expression
                        );

                        return match ($variableRight->getType()) {
                            Types::T_INT,
                            Types::T_UINT,
                            Types::T_LONG,
                            Types::T_ULONG,
                            Types::T_BOOL     => new CompiledExpression(
                                'double',
                                'zephir_safe_mod_double_long('
                                . $left->getCode() . ', ' . $variableRight->getName() . ')',
                                $expression
                            ),
                            Types::T_DOUBLE   => new CompiledExpression(
                                'double',
                                'zephir_safe_mod_double_double('
                                . $left->getCode() . ', ' . $variableRight->getName() . ')',
                                $expression
                            ),
                            Types::T_VARIABLE => new CompiledExpression(
                                'double',
                                'zephir_safe_mod_double_zval('
                                . $left->getCode()
                                . ', '
                                . $this->getIsLocal($variableRight)
                                . $variableRight->getName() . ')',
                                $expression
                            ),
                            default           => throw new CompilerException(
                                "Cannot operate variable('double') with variable('"
                                . $variableRight->getType()
                                . "')",
                                $expression
                            ),
                        };


                    default:
                        throw new CompilerException(
                            "Cannot operate 'double' with '" . $right->getType() . "'",
                            $expression
                        );
                }


            case Types::T_STRING:
            case Types::T_ARRAY:
                throw new CompilerException(
                    'Operation is not supported between ' . $right->getType(),
                    $expression
                );

            case Types::T_VARIABLE:
                $variableLeft = $compilationContext->symbolTable->getVariableForRead(
                    $left->resolve(null, $compilationContext),
                    $compilationContext,
                    $expression
                );
                switch ($variableLeft->getType()) {
                    case Types::T_INT:
                    case Types::T_UINT:
                    case Types::T_LONG:
                    case Types::T_ULONG:
                        switch ($right->getType()) {
                            case Types::T_INT:
                            case Types::T_UINT:
                            case Types::T_LONG:
                            case Types::T_ULONG:
                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_mod_long_long(' . $left->getCode() . ', ' . $right->getCode() . ')',
                                    $expression
                                );

                            case Types::T_DOUBLE:
                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_mod_long_double(' . $left->getCode() . ', ' . $right->getCode() . ')',
                                    $expression
                                );

                            case Types::T_VARIABLE:
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['right']
                                );

                                return match ($variableRight->getType()) {
                                    Types::T_INT,
                                    Types::T_UINT,
                                    Types::T_LONG,
                                    Types::T_ULONG,
                                    Types::T_BOOL     => new CompiledExpression(
                                        'double',
                                        'zephir_safe_mod_long_long('
                                        . $variableLeft->getName() . ', ' . $variableRight->getName() . ')',
                                        $expression
                                    ),
                                    Types::T_DOUBLE   => new CompiledExpression(
                                        'double',
                                        'zephir_safe_mod_long_double('
                                        . $variableLeft->getName() . ', ' . $variableRight->getName() . ')',
                                        $expression
                                    ),
                                    Types::T_VARIABLE => new CompiledExpression(
                                        'double',
                                        'zephir_safe_mod_long_zval('
                                        . $variableLeft->getName()
                                        . ', '
                                        . $this->getIsLocal($variableRight)
                                        . $variableRight->getName() . ')',
                                        $expression
                                    ),
                                    default           => throw new CompilerException(
                                        "Cannot operate variable('int') with variable('"
                                        . $variableRight->getType()
                                        . "')",
                                        $expression
                                    ),
                                };


                            default:
                                throw new CompilerException(
                                    "Cannot operate variable('int') with '" . $right->getType() . "'",
                                    $expression
                                );
                        }


                    case Types::T_BOOL:
                        switch ($right->getType()) {
                            case Types::T_INT:
                            case Types::T_UINT:
                            case Types::T_LONG:
                            case Types::T_ULONG:
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getCode() . ', ' . $right->getCode() . ')',
                                    $expression
                                );

                            case Types::T_BOOL:
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getCode() . ' ' . $this->bitOperator . ' ' . $right->getBooleanCode(
                                    ) . ')',
                                    $expression
                                );

                            case Types::T_VARIABLE:
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['right']
                                );
                                switch ($variableRight->getType()) {
                                    case Types::T_INT:
                                    case Types::T_UINT:
                                    case Types::T_LONG:
                                    case Types::T_ULONG:
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_mod_long_long('
                                            . $variableLeft->getName()
                                            . ', '
                                            . $variableRight->getName()
                                            . ')',
                                            $expression
                                        );

                                    case Types::T_DOUBLE:
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_mod_long_double('
                                            . $variableLeft->getName()
                                            . ', '
                                            . $variableRight->getName()
                                            . ')',
                                            $expression
                                        );

                                    case Types::T_BOOL:
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_mod_long_long('
                                            . $variableLeft->getName()
                                            . ' '
                                            . $this->bitOperator
                                            . ' '
                                            . $variableRight->getName()
                                            . ')',
                                            $expression
                                        );

                                    case Types::T_VARIABLE:
                                        $compilationContext->headersManager->add('kernel/operators');
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_mod_long_zval('
                                            . $variableLeft->getName()
                                            . ', '
                                            . $this->getIsLocal($variableRight)
                                            . $variableRight->getName()
                                            . ')',
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            "Cannot operate variable('int') with variable('"
                                            . $variableRight->getType()
                                            . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot operate variable('int') with '" . $right->getType() . "'",
                                    $expression
                                );
                        }


                    case Types::T_DOUBLE:
                        switch ($right->getType()) {
                            case Types::T_INT:
                            case Types::T_UINT:
                            case Types::T_LONG:
                            case Types::T_ULONG:
                            case Types::T_DOUBLE:
                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_mod_double_long(' . $left->getCode() . ', ' . $right->getCode() . ')',
                                    $expression
                                );

                            case Types::T_BOOL:
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getCode() . ' ' . $this->bitOperator . ' ' . $right->getBooleanCode(
                                    ) . ')',
                                    $expression
                                );

                            case Types::T_VARIABLE:
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->getCode(),
                                    $compilationContext,
                                    $expression['right']
                                );
                                switch ($variableRight->getType()) {
                                    case Types::T_INT:
                                    case Types::T_UINT:
                                    case Types::T_LONG:
                                    case Types::T_ULONG:
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_mod_double_long(' . $variableLeft->getName(
                                            ) . ',  (double) ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case Types::T_DOUBLE:
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_mod_double_long(' . $variableLeft->getName(
                                            ) . ', ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case Types::T_BOOL:
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->bitOperator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case Types::T_VARIABLE:
                                        $compilationContext->headersManager->add('kernel/operators');
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_mod_double_zval('
                                            . $variableLeft->getName()
                                            . ', '
                                            . $this->getIsLocal($variableRight)
                                            . $variableRight->getName() . ')',
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


                    case Types::T_STRING:
                    case Types::T_ARRAY:
                        throw new CompilerException(
                            'Cannot operate ' . $variableLeft->getType() . " variables'",
                            $expression
                        );
                    case Types::T_VARIABLE:
                        $op1 = $compilationContext->backend->getVariableCode($variableLeft);
                        switch ($right->getType()) {
                            /* a + 1 */
                            case Types::T_INT:
                            case Types::T_UINT:
                            case Types::T_LONG:
                            case Types::T_ULONG:
                                $op2 = $right->getCode();

                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_mod_zval_long(' . $op1 . ', ' . $op2 . ')',
                                    $expression
                                );


                            case Types::T_DOUBLE:
                                $op2 = $right->getCode();

                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_mod_zval_double(' . $op1 . ', ' . $op2 . ')',
                                    $expression
                                );


                            /* a(var) + a(x) */
                            case Types::T_VARIABLE:
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->resolve(null, $compilationContext),
                                    $compilationContext,
                                    $expression
                                );
                                switch ($variableRight->getType()) {
                                    /* a(var) + a(int) */
                                    case Types::T_INT:
                                    case Types::T_UINT:
                                    case Types::T_LONG:
                                    case Types::T_ULONG:
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_mod_zval_long('
                                            . $op1 . ', ' . $variableRight->getName() . ')',
                                            $expression
                                        );


                                    /* a(var) + a(bool) */
                                    case Types::T_BOOL:
                                        return new CompiledExpression(
                                            'int',
                                            'zephir_safe_mod_zval_long('
                                            . $op1 . ', ' . $variableRight->getName() . ')',
                                            $expression
                                        );


                                    /* a(var) + a(var) */
                                    case Types::T_VARIABLE:
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $op2 = $compilationContext->backend->getVariableCode($variableRight);

                                        $expected = $this->getExpected($compilationContext, $expression);
                                        $symbol   = $compilationContext->backend->getVariableCode($expected);
                                        $compilationContext->codePrinter->output(
                                            $this->zvalOperator . '(' . $symbol . ', ' . $op1 . ', ' . $op2 . ');'
                                        );

                                        $this->checkVariableTemporal($variableLeft);
                                        $this->checkVariableTemporal($variableRight);

                                        return new CompiledExpression(
                                            'variable',
                                            $expected->getName(),
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            "Cannot operate 'variable' with variable ('"
                                            . $variableRight->getType() . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot operate 'variable' with '"
                                    . $right->getType() . "'",
                                    $expression
                                );
                        }

                    default:
                        throw CompilerException::unknownType($variableLeft, $expression);
                }

            default:
                throw CompilerException::unsupportedType($left, $expression);
        }
    }
}
