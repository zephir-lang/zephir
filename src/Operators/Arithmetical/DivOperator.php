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
use Zephir\Expression;
use Zephir\Types\Types;

/**
 * Generates an arithmetical operation according to the operands
 */
class DivOperator extends ArithmeticalBaseOperator
{
    protected string $bitOperator  = '-';
    protected string $operator     = '/';
    protected string $zvalOperator = 'div_function';

    /**
     * Compiles the arithmetical division operation.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $this->checkLeft($expression);
        $this->checkRight($expression);

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly(true);
        $right = $rightExpr->compile($compilationContext);

        $compilationContext->headersManager->add('kernel/operators');

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
                            'double',
                            'zephir_safe_div_long_long(' . $left->getCode() . ', ' . $right->getCode() . ')',
                            $expression
                        );

                    case 'double':
                        return new CompiledExpression(
                            'double',
                            'zephir_safe_div_long_double((double) ' . $left->getCode() . ', ' . $right->getCode() . ')',
                            $expression
                        );

                    case 'bool':
                        return new CompiledExpression(
                            'bool',
                            '(' . $left->getCode() . ' - ' . $right->getBooleanCode() . ')',
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
                                    'zephir_safe_div_long_long(' . $left->getCode() . ', ' . $variableRight->getName(
                                    ) . ')',
                                    $expression
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_long_double(' . $left->getCode() . ', ' . $variableRight->getName(
                                    ) . ')',
                                    $expression
                                );

                            case 'variable':
                                $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);

                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_long_zval(' . $left->getCode() . ', ' . $variableRightCode . ')',
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
                return $this->processLeftBoolean($right, $left, $expression);


            case 'double':
                switch ($right->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        return new CompiledExpression(
                            'double',
                            'zephir_safe_div_double_long(' . $left->getCode() . ', (double) (' . $right->getCode(
                            ) . '))',
                            $expression
                        );

                    case 'double':
                        return new CompiledExpression(
                            'double',
                            'zephir_safe_div_double_long(' . $left->getCode() . ', ' . $right->getCode() . ')',
                            $expression
                        );

                    case 'bool':
                        return new CompiledExpression(
                            'double',
                            'zephir_safe_div_double_long(' . $left->getCode() . ', ' . $right->getBooleanCode() . ')',
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
                                    'zephir_safe_div_double_long(' . $left->getCode() . ', ' . $variableRight->getName(
                                    ) . ')',
                                    $expression
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_double_double(' . $left->getCode(
                                    ) . ', ' . $variableRight->getName() . ')',
                                    $expression
                                );

                            case 'variable':
                                $symbolRight = $compilationContext->backend->getVariableCode($variableRight);

                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_double_zval(' . $left->getCode() . ', ' . $symbolRight . ')',
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
            case 'array':
                throw match ($right->getType()) {
                    default => new CompilerException(
                        'Operation is not supported between ' . $right->getType(),
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
                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_long_long(' . $left->getCode() . ', ' . $right->getCode() . ')',
                                    $expression
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_long_double(' . $left->getCode() . ', ' . $right->getCode() . ')',
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
                                            'double',
                                            'zephir_safe_div_long_long(' . $variableLeft->getName(
                                            ) . ', ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_div_long_double(' . $variableLeft->getName(
                                            ) . ', ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableRight->isLocalOnly()) {
                                            return new CompiledExpression(
                                                'double',
                                                'zephir_safe_div_long_zval(' . $variableLeft->getName(
                                                ) . ', &' . $variableRight->getName() . ')',
                                                $expression
                                            );
                                        } else {
                                            $variableRightCode = $compilationContext->backend->getVariableCode(
                                                $variableRight
                                            );

                                            return new CompiledExpression(
                                                'double',
                                                'zephir_safe_div_long_zval(' . $variableLeft->getName(
                                                ) . ', ' . $variableRightCode . ')',
                                                $expression
                                            );
                                        }


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


                    case 'bool':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getCode() . ', ' . $right->getCode() . ')',
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
                                            'zephir_safe_div_long_long(' . $variableLeft->getName(
                                            ) . ', ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_div_long_double(' . $variableLeft->getName(
                                            ) . ', ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'bool':
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_div_long_long(' . $variableLeft->getName(
                                            ) . ' ' . $this->bitOperator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableRight->isLocalOnly()) {
                                            return new CompiledExpression(
                                                'double',
                                                'zephir_safe_div_long_zval(' . $variableLeft->getName(
                                                ) . ', &' . $variableRight->getName() . ')',
                                                $expression
                                            );
                                        } else {
                                            $variableRightCode = $compilationContext->backend->getVariableCode(
                                                $variableRight
                                            );

                                            return new CompiledExpression(
                                                'double',
                                                'zephir_safe_div_long_zval(' . $variableLeft->getName(
                                                ) . ', ' . $variableRightCode . ')',
                                                $expression
                                            );
                                        }


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
                            case 'double':
                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_double_long(' . $left->getCode() . ', ' . $right->getCode() . ')',
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
                                            'zephir_safe_div_double_long(' . $variableLeft->getName(
                                            ) . ',  (double) ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_div_double_long(' . $variableLeft->getName(
                                            ) . ', ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'bool':
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->bitOperator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $symbolRight = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_div_double_zval(' . $variableLeft->getName(
                                            ) . ', ' . $symbolRight . ')',
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
                        throw new CompilerException("Cannot operate array variables'", $expression);
                    case 'variable':
                        $op1 = $compilationContext->backend->getVariableCode($variableLeft);
                        switch ($right->getType()) {
                            /* a + 1 */
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                $op2 = $right->getCode();

                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_zval_long(' . $op1 . ', ' . $op2 . ')',
                                    $expression
                                );

                            case 'double':
                                $op2 = $right->getCode();

                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_zval_double(' . $op1 . ', ' . $op2 . ')',
                                    $expression
                                );

                            /* a(var) + a(x) */
                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->resolve(null, $compilationContext),
                                    $compilationContext,
                                    $expression
                                );
                                switch ($variableRight->getType()) {
                                    /* a(var) + a(int) */
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_div_zval_long(' . $op1 . ', ' . $variableRight->getName(
                                            ) . ')',
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_div_zval_double(' . $op1 . ', ' . $variableRight->getName(
                                            ) . ')',
                                            $expression
                                        );

                                    /* a(var) + a(bool) */
                                    case 'bool':
                                        return new CompiledExpression(
                                            'int',
                                            'zephir_safe_div_zval_long(' . $op1 . ', ' . $variableRight->getName(
                                            ) . ')',
                                            $expression
                                        );

                                    /* a(var) + a(var) */
                                    case 'variable':
                                        $op2 = $compilationContext->backend->getVariableCode($variableRight);

                                        $expected     = $this->getExpected($compilationContext, $expression);
                                        $expectedCode = $compilationContext->backend->getVariableCode($expected);
                                        $compilationContext->codePrinter->output(
                                            $this->zvalOperator . '(' . $expectedCode . ', ' . $op1 . ', ' . $op2 . ');'
                                        );

                                        if ($variableLeft->isTemporal()) {
                                            $variableLeft->setIdle(true);
                                        }
                                        if ($variableRight->isTemporal()) {
                                            $variableRight->setIdle(true);
                                        }

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


                    case 'variable':
                        $op1 = $compilationContext->backend->getVariableCode($variableLeft);
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                $op2 = $right->getCode();

                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_zval_long(' . $op1 . ', ' . $op2 . ')',
                                    $expression
                                );

                            case 'double':
                                $op2 = $right->getCode();

                                return new CompiledExpression(
                                    'double',
                                    'zephir_safe_div_zval_double(' . $op1 . ', ' . $op2 . ')',
                                    $expression
                                );

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->resolve(null, $compilationContext),
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
                                            'zephir_safe_div_zval_long(' . $op1 . ', ' . $variableRight->getName(
                                            ) . ')',
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'double',
                                            'zephir_safe_div_zval_double(' . $op1 . ', ' . $variableRight->getName(
                                            ) . ')',
                                            $expression
                                        );

                                    case 'variable':
                                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                            $variableRight->getCode(),
                                            $compilationContext,
                                            $expression
                                        );
                                        $op2           = $compilationContext->backend->getVariableCode($variableRight);
                                        switch ($variableRight->getType()) {
                                            case 'int':
                                                return new CompiledExpression(
                                                    'double',
                                                    'zephir_safe_div_zval_long(' . $variableLeft->getName(
                                                    ) . ', ' . $variableRight->getName() . ')',
                                                    $expression
                                                );

                                            case 'double':
                                                return new CompiledExpression(
                                                    'double',
                                                    'zephir_safe_div_zval_double(' . $variableLeft->getName(
                                                    ) . ', ' . $variableRight->getName() . ')',
                                                    $expression
                                                );

                                            case 'bool':
                                                return new CompiledExpression(
                                                    'bool',
                                                    $variableLeft->getName(
                                                    ) . ' ' . $this->bitOperator . ' ' . $variableRight->getName(),
                                                    $expression
                                                );

                                            case 'variable':
                                                $expected     = $this->getExpected($compilationContext, $expression);
                                                $expectedCode = $compilationContext->backend->getVariableCode(
                                                    $expected
                                                );
                                                $compilationContext->codePrinter->output(
                                                    $this->zvalOperator . '(' . $expectedCode . ', ' . $op1 . ', ' . $op2 . ');'
                                                );

                                                return new CompiledExpression(
                                                    'variable',
                                                    $expected->getName(),
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
                        throw new CompilerException("Unknown '" . $variableLeft->getType() . "'", $expression);
                }


            default:
                throw new CompilerException('Unsupported type: ' . $left->getType(), $expression);
        }
    }
}
