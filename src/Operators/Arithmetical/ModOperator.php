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
use Zephir\Variable\Variable;

/**
 * Generates an arithmetical operation according to the operands
 */
class ModOperator extends ArithmeticalBaseOperator
{
    protected string $bitOperator        = '-';
    protected string $operator           = '%';
    protected string $zephirDoubleDouble = 'zephir_safe_mod_double_double';
    protected string $zephirDoubleLong   = 'zephir_safe_mod_double_long';
    protected string $zephirLongDouble   = 'zephir_safe_mod_long_double';
    protected string $zephirLongLong     = 'zephir_safe_mod_long_long';
    protected string $zvalOperator       = 'mod_function';

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
                return $this->processLeftInt(
                    $right,
                    $left,
                    $expression,
                    $compilationContext
                );

            case Types::T_BOOL:
                return $this->processLeftBoolean($left, $right, $expression);

            case Types::T_DOUBLE:
                return $this->processLeftDouble(
                    $right,
                    $left,
                    $expression,
                    $compilationContext
                );


            case Types::T_STRING:
            case Types::T_ARRAY:
                $this->processLeftStringArray($right, $expression);
            // no break

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
                                    $this->zephirLongLong . '(' . $left->getCode() . ', ' . $right->getCode() . ')',
                                    $expression
                                );

                            case Types::T_DOUBLE:
                                return new CompiledExpression(
                                    'double',
                                    $this->zephirLongDouble . '(' . $left->getCode() . ', ' . $right->getCode() . ')',
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
                                        $this->zephirLongLong . '(' . $variableLeft->getName(
                                        ) . ', ' . $variableRight->getName() . ')',
                                        $expression
                                    ),
                                    Types::T_DOUBLE   => new CompiledExpression(
                                        'double',
                                        $this->zephirLongDouble . '(' . $variableLeft->getName(
                                        ) . ', ' . $variableRight->getName() . ')',
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
                                            $this->zephirLongLong . '('
                                            . $variableLeft->getName()
                                            . ', '
                                            . $variableRight->getName()
                                            . ')',
                                            $expression
                                        );

                                    case Types::T_DOUBLE:
                                        return new CompiledExpression(
                                            'double',
                                            $this->zephirLongDouble . '('
                                            . $variableLeft->getName()
                                            . ', '
                                            . $variableRight->getName()
                                            . ')',
                                            $expression
                                        );

                                    case Types::T_BOOL:
                                        return new CompiledExpression(
                                            'double',
                                            $this->zephirLongLong . '('
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
                                    $this->zephirDoubleLong . '(' . $left->getCode() . ', ' . $right->getCode() . ')',
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
                                            $this->zephirDoubleLong . '(' . $variableLeft->getName(
                                            ) . ',  (double) ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case Types::T_DOUBLE:
                                        return new CompiledExpression(
                                            'double',
                                            $this->zephirDoubleLong . '(' . $variableLeft->getName(
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
                                            'zephir_safe_mod_zval_long(' . $op1 . ', ' . $variableRight->getName(
                                            ) . ')',
                                            $expression
                                        );


                                    /* a(var) + a(bool) */
                                    case Types::T_BOOL:
                                        return new CompiledExpression(
                                            'int',
                                            'zephir_safe_mod_zval_long(' . $op1 . ', ' . $variableRight->getName(
                                            ) . ')',
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

                                        if ($variableLeft->isTemporal()) {
                                            $variableLeft->setIdle(true);
                                        }
                                        if ($variableRight->isTemporal()) {
                                            $variableRight->setIdle(true);
                                        }

                                        return new CompiledExpression(
                                            'variable',
                                            $expected->getName(),
                                            $expression
                                        );

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


                    case Types::T_VARIABLE:
                        $op1 = $compilationContext->backend->getVariableCode($variableLeft);
                        switch ($right->getType()) {
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

                            case Types::T_VARIABLE:
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->resolve(null, $compilationContext),
                                    $compilationContext,
                                    $expression
                                );
                                switch ($variableRight->getType()) {
                                    case Types::T_INT:
                                    case Types::T_UINT:
                                    case Types::T_LONG:
                                    case Types::T_ULONG:
                                    case Types::T_BOOL:
                                        $compilationContext->headersManager->add('kernel/operators');

                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_mod_zval_long(' . $op1 . ', ' . $variableRight->getName(
                                            ) . ')',
                                            $expression
                                        );


                                    case Types::T_VARIABLE:
                                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                            $variableRight->getCode(),
                                            $compilationContext,
                                            $expression
                                        );
                                        switch ($variableRight->getType()) {
                                            case Types::T_INT:
                                                return new CompiledExpression(
                                                    'double',
                                                    'zephir_safe_mod_zval_long(' . $op1 . ', ' . $variableRight->getName(
                                                    ) . ')',
                                                    $expression
                                                );

                                            case Types::T_DOUBLE:
                                                return new CompiledExpression(
                                                    'double',
                                                    'zephir_safe_mod_zval_double(' . $op1 . ', ' . $variableRight->getName(
                                                    ) . ')',
                                                    $expression
                                                );

                                            case Types::T_BOOL:
                                                return new CompiledExpression(
                                                    'bool',
                                                    $op1 . ' ' . $this->bitOperator . ' ' . $variableRight->getName(),
                                                    $expression
                                                );

                                            case Types::T_VARIABLE:
                                                $op2 = $compilationContext->backend->getVariableCode($variableRight);

                                                $expected = $this->getExpected($compilationContext, $expression);
                                                $symbol   = $compilationContext->backend->getVariableCode($expected);
                                                $compilationContext->codePrinter->output(
                                                    $this->zvalOperator . '(' . $symbol . ', ' . $op1 . ', ' . $op2 . ');'
                                                );

                                                return new CompiledExpression(
                                                    'variable',
                                                    $expected->getName(),
                                                    $expression
                                                );

                                            default:
                                                throw new CompilerException(
                                                    "Cannot operate variable('double') with variable('"
                                                    . $variableRight->getType() . "')",
                                                    $expression
                                                );
                                        }


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
     * @param CompilationContext $compilationContext
     * @param Variable|bool      $variableRight
     * @param mixed              $left
     * @param array              $expression
     *
     * @return CompiledExpression
     */
    protected function processDoubleVarVar(
        CompilationContext $compilationContext,
        mixed $variableRight,
        mixed $left,
        array $expression
    ): CompiledExpression {
        return new CompiledExpression(
            'double',
            'zephir_safe_mod_double_zval('
            . $left->getCode()
            . ', '
            . $this->getIsLocal($variableRight)
            . $variableRight->getName() . ')',
            $expression
        );
    }

    /**
     * @param CompilationContext $compilationContext
     * @param Variable|bool      $variableRight
     * @param mixed              $left
     * @param array              $expression
     *
     * @return CompiledExpression
     */
    protected function processIntVarVar(
        CompilationContext $compilationContext,
        mixed $variableRight,
        mixed $left,
        array $expression
    ): CompiledExpression {
        return new CompiledExpression(
            'double',
            'zephir_safe_mod_long_zval('
            . $left->getCode()
            . ', '
            . $this->getIsLocal($variableRight)
            . $variableRight->getName() . ')',
            $expression
        );
    }

    /**
     * @param mixed              $right
     * @param mixed              $left
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     */
    protected function processLeftDouble(
        mixed $right,
        mixed $left,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        switch ($right->getType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
                return new CompiledExpression(
                    'double',
                    $this->zephirDoubleLong
                    . '(' . $left->getCode() . ', (double) (' . $right->getCode() . '))',
                    $expression
                );

            case Types::T_DOUBLE:
                return new CompiledExpression(
                    'double',
                    $this->zephirDoubleLong
                    . '(' . $left->getCode() . ', ' . $right->getCode() . ')',
                    $expression
                );

            case Types::T_BOOL:
                return new CompiledExpression(
                    'double',
                    $this->zephirDoubleLong
                    . '(' . $left->getCode() . ', ' . $right->getBooleanCode() . ')',
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
                        $this->zephirDoubleLong
                        . '(' . $left->getCode() . ', ' . $variableRight->getName() . ')',
                        $expression
                    ),
                    Types::T_DOUBLE   => new CompiledExpression(
                        'double',
                        $this->zephirDoubleDouble
                        . '(' . $left->getCode() . ', ' . $variableRight->getName() . ')',
                        $expression
                    ),
                    Types::T_VARIABLE => $this->processDoubleVarVar(
                        $compilationContext,
                        $variableRight,
                        $left,
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
    }

    /**
     * @param mixed              $right
     * @param mixed              $left
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     */
    protected function processLeftInt(
        mixed $right,
        mixed $left,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        switch ($right->getType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
                return new CompiledExpression(
                    'double',
                    $this->zephirLongLong
                    . '(' . $left->getCode() . ', ' . $right->getCode() . ')',
                    $expression
                );

            case Types::T_DOUBLE:
                return new CompiledExpression(
                    'double',
                    $this->zephirLongDouble
                    . '((double) ' . $left->getCode() . ', ' . $right->getCode() . ')',
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
                        $this->zephirLongLong
                        . '(' . $left->getCode() . ', ' . $variableRight->getName() . ')',
                        $expression
                    ),
                    Types::T_DOUBLE   => new CompiledExpression(
                        'double',
                        $this->zephirLongDouble
                        . '(' . $left->getCode() . ', ' . $variableRight->getName() . ')',
                        $expression
                    ),
                    Types::T_VARIABLE => $this->processIntVarVar(
                        $compilationContext,
                        $left,
                        $variableRight,
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
    }

    /**
     * @param mixed $right
     * @param array $expression
     *
     * @return mixed
     */
    protected function processLeftStringArray(mixed $right, array $expression)
    {
        throw match ($right->getType()) {
            default => new CompilerException(
                'Operation is not supported between ' . $right->getType(),
                $expression
            ),
        };
    }
}
