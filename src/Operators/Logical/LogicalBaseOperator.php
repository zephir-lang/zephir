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

namespace Zephir\Operators\Logical;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;
use Zephir\Types\Types;

/**
 * This is the base operator for logical operators
 */
class LogicalBaseOperator extends AbstractOperator
{
    public function compile($expression, CompilationContext $compilationContext): CompiledExpression
    {
        $this->checkLeft($expression, CompilerException::class, $expression);
        $this->checkRight($expression, CompilerException::class, $expression);

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->readOnly);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly($this->readOnly);
        $right = $rightExpr->compile($compilationContext);

        switch ($left->getType()) {
            case 'int':
                switch ($right->getType()) {
                    case 'int':
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
                            case 'bool':
                            case 'double':
                            case 'int':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getCode() . ' ' . $this->operator . ' ' . $variableRight->getName(
                                    ) . ')',
                                    $expression
                                );

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $variableCode = $compilationContext->backend->getVariableCode($variableRight);

                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getCode(
                                    ) . ' ' . $this->operator . ' zephir_is_true(' . $variableCode . '))',
                                    $expression
                                );

                            default:
                                throw new CompilerException(
                                    "Cannot compare variable('int') with variable('" . $variableRight->getType() . "')",
                                    $expression
                                );
                        }


                    default:
                        throw new CompilerException(
                            "Cannot compare 'int' with '" . $right->getType() . "'",
                            $expression
                        );
                }


            case 'bool':
                switch ($right->getType()) {
                    case 'int':
                    case 'double':
                        return new CompiledExpression(
                            'bool',
                            '(' . $left->getBooleanCode() . ' ((' . $right->getCode() . ') ? 1 : 0))',
                            $expression
                        );

                    case 'bool':
                        return new CompiledExpression(
                            'bool',
                            '(' . $left->getBooleanCode() . ' ' . $right->getBooleanCode() . ')',
                            $expression
                        );

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                            $right->getCode(),
                            $compilationContext,
                            $expression
                        );
                        switch ($variableRight->getType()) {
                            case 'bool':
                            case 'double':
                            case 'int':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getBooleanCode(
                                    ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                    $expression
                                );

                            case 'variable':
                                $variableCode = $compilationContext->backend->getVariableCode($variableRight);
                                $compilationContext->headersManager->add('kernel/operators');

                                return new CompiledExpression(
                                    'bool',
                                    '(' . $left->getBooleanCode(
                                    ) . ' ' . $this->operator . ' zephir_is_true(' . $variableCode . '))',
                                    $expression
                                );

                            default:
                                throw new CompilerException(
                                    "Cannot add variable('int') with variable('" . $variableRight->getType() . "')",
                                    $expression
                                );
                        }


                    default:
                        throw new CompilerException(
                            "Cannot compare 'bool' with '" . $right->getType() . "'",
                            $expression
                        );
                }

            case 'double':
                return match ($right->getType()) {
                    Types::T_DOUBLE,
                    Types::T_INT  => new CompiledExpression(
                        'bool',
                        '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode() . ')',
                        $expression
                    ),
                    Types::T_BOOL => new CompiledExpression(
                        'bool',
                        '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getBooleanCode() . ')',
                        $expression
                    ),
                    default       => throw new CompilerException(
                        "Cannot compare 'double' with '" . $right->getType() . "'",
                        $expression
                    ),
                };


            case 'string':
                throw new CompilerException('Operation is not supported between strings', $expression);


            case 'variable':
                $variableLeft = $compilationContext->symbolTable->getVariableForRead(
                    $left->resolve(null, $compilationContext),
                    $compilationContext,
                    $expression
                );
                switch ($variableLeft->getType()) {
                    case 'int':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $variableLeft->getName() . ' ' . $this->operator . ' ' . $right->getCode(
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
                                    case 'bool':
                                    case 'double':
                                    case 'int':
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression(
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' zephir_is_true(' . $variableCode . '))',
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            "Cannot compare variable('int') with variable('" . $variableRight->getType(
                                            ) . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot compare variable('int') with '" . $right->getType() . "'",
                                    $expression
                                );
                        }


                    case 'bool':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $variableLeft->getName() . ' ' . $this->operator . ' ' . $right->getCode(
                                    ) . ')',
                                    $expression
                                );

                            case 'bool':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $variableLeft->getName() . ' ' . $right->getBooleanCode() . ')',
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
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'bool':
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName() . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' zephir_is_true(' . $variableCode . '))',
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            "Cannot compare variable('int') with variable('" . $variableRight->getType(
                                            ) . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot compare variable('int') with '" . $right->getType() . "'",
                                    $expression
                                );
                        }


                    case 'double':
                        switch ($right->getType()) {
                            case 'double':
                            case 'int':
                                return new CompiledExpression(
                                    'bool',
                                    $variableLeft->getName() . ' ' . $this->operator . ' ' . $right->getCode(),
                                    $expression
                                );

                            case 'bool':
                                return new CompiledExpression(
                                    'bool',
                                    $variableLeft->getName() . ' ' . $right->getBooleanCode(),
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
                                        return new CompiledExpression(
                                            'bool',
                                            $variableLeft->getName(
                                            ) . ' ' . $this->operator . '  (double) ' . $variableRight->getName(),
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'bool',
                                            $variableLeft->getName(
                                            ) . ' ' . $this->operator . '  ' . $variableRight->getName(),
                                            $expression
                                        );

                                    case 'bool':
                                        return new CompiledExpression(
                                            'bool',
                                            $variableLeft->getName() . ' ' . $variableRight->getName(),
                                            $expression
                                        );

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression(
                                            'bool',
                                            $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' zephir_is_true(' . $variableCode . ')',
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            "Cannot compare variable('double') with variable('" . $variableRight->getType(
                                            ) . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot compare variable('int') with '" . $right->getType() . "'",
                                    $expression
                                );
                        }


                    case 'string':
                        switch ($right->getType()) {
                            case 'double':
                            case 'int':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName(
                                    ) . ')) ' . $this->operator . ' ' . $right->getCode(),
                                    $expression
                                );

                            case 'bool':
                                return new CompiledExpression(
                                    'bool',
                                    '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName(
                                    ) . ')) ' . $right->getBooleanCode(),
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
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName(
                                            ) . ')) ' . $this->operator . ' ' . $variableRight->getName(),
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName(
                                            ) . ')) ' . $this->operator . '  ' . $variableRight->getName(),
                                            $expression
                                        );

                                    case 'string':
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName(
                                            ) . ')) ' . $this->operator . ' (' . $variableRight->getName(
                                            ) . ' && Z_STRLEN_P(' . $variableRight->getName() . '))',
                                            $expression
                                        );

                                    case 'bool':
                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName(
                                            ) . ')) ' . $variableRight->getName(),
                                            $expression
                                        );

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression(
                                            'bool',
                                            '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName(
                                            ) . '))  ' . $this->operator . ' zephir_is_true(' . $variableCode . ')',
                                            $expression
                                        );

                                    default:
                                        throw new CompilerException(
                                            "Cannot compare variable('double') with variable('" . $variableRight->getType(
                                            ) . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot compare variable('int') with '" . $right->getType() . "'",
                                    $expression
                                );
                        }


                    case 'variable':
                        $variableLeftCode = $compilationContext->backend->getVariableCode($variableLeft);
                        switch ($right->getType()) {
                            /* a && 1 */
                            case 'int':
                            case 'bool':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                $op  = $this->operator;
                                $op1 = $variableLeftCode;
                                $op2 = $right->getCode();
                                $compilationContext->headersManager->add('kernel/operators');

                                return new CompiledExpression(
                                    'bool',
                                    'zephir_is_true(' . $op1 . ') ' . $op . ' ' . $op2,
                                    $expression
                                );

                            /* a(var) && a(x) */
                            case 'variable':
                                $variableRight     = $compilationContext->symbolTable->getVariableForRead(
                                    $right->resolve(null, $compilationContext),
                                    $compilationContext,
                                    $expression
                                );
                                $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);
                                switch ($variableRight->getType()) {
                                    /* a(var) && a(int) */
                                    /* a(var) && a(bool) */
                                    case 'bool':
                                    case 'int':
                                        $compilationContext->headersManager->add('kernel/operators');

                                        return new CompiledExpression(
                                            'bool',
                                            'zephir_is_true(' . $variableLeftCode . ') ' . $this->operator . ' ' . $variableRightCode,
                                            $expression
                                        );

                                    /* a(var) && a(var) */
                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $op1 = $variableLeftCode;
                                        $op2 = $variableRightCode;

                                        $expected = $this->getExpected($compilationContext, $expression);
                                        if ($expected->isLocalOnly()) {
                                            $compilationContext->codePrinter->output(
                                                'add_function(&' . $expected->getName(
                                                ) . ', ' . $op1 . ', ' . $op2 . ');'
                                            );
                                        } else {
                                            $compilationContext->codePrinter->output(
                                                'add_function(' . $expected->getName(
                                                ) . ', ' . $op1 . ', ' . $op2 . ');'
                                            );
                                        }

                                        return new CompiledExpression('variable', $expected->getName(), $expression);

                                    default:
                                        throw new CompilerException(
                                            "Cannot compare 'variable' with variable ('" . $variableRight->getType(
                                            ) . "')",
                                            $expression
                                        );
                                }


                            default:
                                throw new CompilerException(
                                    "Cannot compare 'variable' with '" . $right->getType() . "'",
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
