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

namespace Zephir\Operators\Bitwise;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;

/**
 * This is the base operator for commutative, associative and distributive
 * arithmetic operators
 */
class BitwiseBaseOperator extends AbstractOperator
{
    protected bool $literalOnly = true;

    /**
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return bool|CompiledExpression
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $this->checkLeft($expression, CompilerException::class, $expression);
        $this->checkRight($expression, CompilerException::class, $expression);

        /**
         * Check for constant folding optimizations.
         */
        $optimized = $this->optimizeConstantFolding($expression, $compilationContext);
        if ($optimized !== null) {
            return $optimized;
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly(true);
        $right = $rightExpr->compile($compilationContext);

        switch ($left->getType()) {
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
                    case 'char':
                    case 'uchar':
                        return new CompiledExpression(
                            'int',
                            '(' . $left->getCode() . ' ' . $this->operator . ' ' . $right->getCode() . ')',
                            $expression
                        );

                    case 'double':
                        return new CompiledExpression(
                            'int',
                            '(' . $left->getCode() . ' ' . $this->operator . ' (int) (' . $right->getCode() . '))',
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
                            case 'bool':
                            case 'ulong':
                                return new CompiledExpression(
                                    'int',
                                    '(' . $left->getCode() . ' ' . $this->operator . ' ' . $variableRight->getName(
                                    ) . ')',
                                    $expression
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'int',
                                    '(' . $left->getCode(
                                    ) . ' ' . $this->operator . ' (int) (' . $variableRight->getName() . '))',
                                    $expression
                                );

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $symbol = $compilationContext->backend->getVariableCode($variableRight);

                                return new CompiledExpression(
                                    'int',
                                    '(' . $left->getCode(
                                    ) . ' ' . $this->operator . ' zephir_get_numberval(' . $symbol . '))',
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
                switch ($right->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'double':
                        return new CompiledExpression(
                            'int',
                            '(' . $left->getBooleanCode() . ' ' . $this->bitOperator . '((' . $right->getCode(
                            ) . ') ? 1 : 0))',
                            $expression
                        );

                    case 'bool':
                        return new CompiledExpression(
                            'int',
                            '(' . $left->getBooleanCode() . ' ' . $this->bitOperator . ' ' . $right->getBooleanCode(
                            ) . ')',
                            $expression
                        );

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                            $expression['right']['value'],
                            $compilationContext,
                            $expression
                        );
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'bool':
                            case 'ulong':
                                return new CompiledExpression(
                                    'int',
                                    '((int) (' . $left->getBooleanCode(
                                    ) . ') ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                    $expression
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'int',
                                    '((int) (' . $left->getBooleanCode(
                                    ) . ') ' . $this->operator . ' (int) (' . $variableRight->getName() . '))',
                                    $expression
                                );

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $symbol = $compilationContext->backend->getVariableCode($variableRight);

                                return new CompiledExpression(
                                    'int',
                                    '((int) (' . $left->getBooleanCode(
                                    ) . ') ' . $this->operator . ' zephir_get_numberval(' . $symbol . '))',
                                    $expression
                                );


                            default:
                                throw new CompilerException(
                                    "Cannot operate ('bool') with variable('" . $variableRight->getType() . "')",
                                    $expression
                                );
                        }


                    default:
                        throw new CompilerException(
                            "Cannot operate 'bool' with '" . $right->getType() . "'",
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
                            'int',
                            '((int) (' . $left->getCode() . ') ' . $this->operator . ' ' . $right->getCode() . ')',
                            $expression
                        );

                    case 'double':
                        return new CompiledExpression(
                            'int',
                            '((int) (' . $left->getCode() . ') ' . $this->operator . ' (int) (' . $right->getCode(
                            ) . '))',
                            $expression
                        );

                    case 'bool':
                        return new CompiledExpression(
                            'int',
                            '((int) (' . $left->getCode() . ') ' . $this->operator . ' ' . $right->getBooleanCode(
                            ) . ')',
                            $expression
                        );

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead(
                            $expression['right']['value'],
                            $compilationContext,
                            $expression
                        );
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'bool':
                            case 'ulong':
                                return new CompiledExpression(
                                    'int',
                                    '((int) (' . $left->getCode(
                                    ) . ') ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                    $expression
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'int',
                                    '((int) (' . $left->getCode(
                                    ) . ') ' . $this->operator . ' (int) (' . $variableRight->getName() . '))',
                                    $expression
                                );

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $symbol = $compilationContext->backend->getVariableCode($variableRight);

                                return new CompiledExpression(
                                    'int',
                                    '((int) (' . $left->getCode(
                                    ) . ') ' . $this->operator . ' zephir_get_numberval(' . $symbol . '))',
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
                    case 'char':
                    case 'uchar':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                            case 'char':
                            case 'uchar':
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
                                    case 'char':
                                    case 'bool':
                                    case 'uchar':
                                        return new CompiledExpression(
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' (int) (' . $variableRight->getName() . '))',
                                            $expression
                                        );

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $symbol = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression(
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' (int) (zephir_get_numberval(' . $symbol . ')))',
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


                    case 'bool':
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

                            case 'bool':
                                return new CompiledExpression(
                                    'int',
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
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'double':
                                        return new CompiledExpression(
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' (int) (' . $variableRight->getName() . '))',
                                            $expression
                                        );

                                    case 'bool':
                                        return new CompiledExpression(
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->bitOperator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $symbol = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression(
                                            'int',
                                            '(' . $variableLeft->getName(
                                            ) . ' ' . $this->operator . ' zephir_get_numberval(' . $symbol . '))',
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
                                    'int',
                                    '((int) (' . $left->getCode() . ') ' . $this->operator . ' ' . $right->getCode(
                                    ) . ')',
                                    $expression
                                );

                            case 'double':
                                return new CompiledExpression(
                                    'int',
                                    '((int) (' . $left->getCode(
                                    ) . ') ' . $this->operator . ' (int) (' . $right->getCode() . '))',
                                    $expression
                                );

                            case 'bool':
                                return new CompiledExpression(
                                    'int',
                                    '((int) (' . $left->getCode(
                                    ) . ') ' . $this->bitOperator . ' ' . $right->getBooleanCode() . ')',
                                    $expression
                                );

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $expression['right']['value'],
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
                                            '((int) (' . $variableLeft->getName(
                                            ) . ') ' . $this->operator . '  ' . $variableRight->getName() . ')',
                                            $expression
                                        );
                                    case 'double':
                                        return new CompiledExpression(
                                            'int',
                                            '((int) (' . $variableLeft->getName(
                                            ) . ') ' . $this->operator . ' (int) (' . $variableRight->getName() . '))',
                                            $expression
                                        );

                                    case 'bool':
                                        return new CompiledExpression(
                                            'int',
                                            '((int) (' . $variableLeft->getName(
                                            ) . ') ' . $this->bitOperator . ' ' . $variableRight->getName() . ')',
                                            $expression
                                        );

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $symbol = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression(
                                            'int',
                                            '((int) (' . $variableLeft->getName(
                                            ) . ') ' . $this->operator . ' (int) (zephir_get_numberval(' . $symbol . ')))',
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
                    case 'variable':
                        switch ($right->getType()) {
                            /* a + 1 */
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                $op  = $this->operator;
                                $op1 = $compilationContext->backend->getVariableCode($variableLeft);
                                $op2 = $right->getCode();
                                if ('double' == $right->getType()) {
                                    return new CompiledExpression(
                                        'int',
                                        '((int) (zephir_get_numberval(' . $op1 . ')) ' . $op . ' (int) (' . $op2 . '))',
                                        $expression
                                    );
                                } else {
                                    return new CompiledExpression(
                                        'int',
                                        '((int) (zephir_get_numberval(' . $op1 . ')) ' . $op . ' ' . $op2 . ')',
                                        $expression
                                    );
                                }


                            /* a(var) + a(x) */
                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead(
                                    $right->resolve(null, $compilationContext),
                                    $compilationContext,
                                    $expression
                                );
                                $symbol        = $compilationContext->backend->getVariableCode($variableLeft);
                                switch ($variableRight->getType()) {
                                    /* a(var) + a(int) */
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    /* a(var) + a(bool) */
                                    case 'bool':
                                        $compilationContext->headersManager->add('kernel/operators');

                                        return new CompiledExpression(
                                            'int',
                                            '((int) (zephir_get_numberval(' . $symbol . ')) ' . $this->operator . ' ' . $variableRight->getName(
                                            ) . ')',
                                            $expression
                                        );

                                    /* a(var) + a(var) */
                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $op1 = $symbol;
                                        $op2 = $compilationContext->backend->getVariableCode($variableRight);

                                        $expected       = $this->getExpected($compilationContext, $expression);
                                        $expectedSymbol = $compilationContext->backend->getVariableCode($expected);
                                        $compilationContext->codePrinter->output(
                                            $this->zvalOperator . '(' . $expectedSymbol . ', ' . $op1 . ', ' . $op2 . ');'
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


                    default:
                        throw CompilerException::unknownType($variableLeft, $expression);
                }


            default:
                throw CompilerException::unsupportedType($left, $expression);
        }
    }

    /**
     * This tries to perform arithmetical operations
     * Probably gcc/clang will optimize them without this optimization.
     *
     * @see https://en.wikipedia.org/wiki/Constant_folding
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression|null
     */
    public function optimizeConstantFolding(
        array $expression,
        CompilationContext $compilationContext
    ): ?CompiledExpression {
        if (!$compilationContext->config->get('constant-folding', 'optimizations')) {
            return null;
        }

        switch ($expression['left']['type']) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
                // continue to next switch
                break;
            default:
                return null;
        }

        switch ($expression['right']['type']) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
                // continue to operator switch
                break;
            default:
                return null;
        }

        /*
         * Return value will be always int
         */
        return match ($this->operator) {
            '&'     => new CompiledExpression(
                'int',
                $expression['left']['value'] & $expression['right']['value'],
                $expression
            ),
            '|'     => new CompiledExpression(
                'int',
                $expression['left']['value'] | $expression['right']['value'],
                $expression
            ),
            '^'     => new CompiledExpression(
                'int',
                $expression['left']['value'] ^ $expression['right']['value'],
                $expression
            ),
            '<<'    => new CompiledExpression(
                'int',
                $expression['left']['value'] << $expression['right']['value'],
                $expression
            ),
            '>>'    => new CompiledExpression(
                'int',
                $expression['left']['value'] >> $expression['right']['value'],
                $expression
            ),
            default => null,
        };
    }
}
