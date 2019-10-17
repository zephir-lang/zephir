<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Logical;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\BaseOperator;

/**
 * LogicalBaseOperator.
 *
 * This is the base operator for logical operators
 */
class LogicalBaseOperator extends BaseOperator
{
    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException('Missing left part of the expression', $expression);
        }

        if (!isset($expression['right'])) {
            throw new CompilerException('Missing right part of the expression', $expression);
        }

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
                        return new CompiledExpression('int', '('.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                    case 'double':
                        return new CompiledExpression('double', '((double) '.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                    case 'bool':
                        return new CompiledExpression('int', '('.$left->getCode().' '.$this->operator.' '.$right->getBooleanCode().')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', '('.$left->getCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '('.$left->getCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'double':
                                return new CompiledExpression('bool', '('.$left->getCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $variableCode = $compilationContext->backend->getVariableCode($variableRight);

                                return new CompiledExpression('bool', '('.$left->getCode().' '.$this->operator.' zephir_is_true('.$variableCode.'))', $expression);

                            default:
                                throw new CompilerException("Cannot compare variable('int') with variable('".$variableRight->getType()."')", $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot compare 'int' with '".$right->getType()."'", $expression);
                }
                break;

            case 'bool':
                switch ($right->getType()) {
                    case 'int':
                    case 'double':
                        return new CompiledExpression('bool', '('.$left->getBooleanCode().' '.$this->bitOperator.' (('.$right->getCode().') ? 1 : 0))', $expression);

                    case 'bool':
                        return new CompiledExpression('bool', '('.$left->getBooleanCode().' '.$this->bitOperator.' '.$right->getBooleanCode().')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', '('.$left->getBooleanCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '('.$left->getBooleanCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'double':
                                return new CompiledExpression('bool', '('.$left->getBooleanCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'variable':
                                $variableCode = $compilationContext->backend->getVariableCode($variableRight);
                                $compilationContext->headersManager->add('kernel/operators');

                                return new CompiledExpression('bool', '('.$left->getBooleanCode().' '.$this->operator.' zephir_is_true('.$variableCode.'))', $expression);

                            default:
                                throw new CompilerException("Cannot add variable('int') with variable('".$variableRight->getType()."')", $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot compare 'bool' with '".$right->getType()."'", $expression);
                }
                break;
            case 'double':
                switch ($right->getType()) {
                    case 'int':
                        return new CompiledExpression('bool', '('.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                    case 'double':
                        return new CompiledExpression('bool', '('.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                    case 'bool':
                        return new CompiledExpression('bool', '('.$left->getCode().' '.$this->operator.' '.$right->getBooleanCode().')', $expression);

                    default:
                        throw new CompilerException("Cannot compare 'double' with '".$right->getType()."'", $expression);
                }
                break;

            case 'string':
                switch ($right->getType()) {
                    default:
                        throw new CompilerException('Operation is not supported between strings', $expression);
                }
                break;

            case 'variable':
                $variableLeft = $compilationContext->symbolTable->getVariableForRead($left->resolve(null, $compilationContext), $compilationContext, $expression);
                switch ($variableLeft->getType()) {
                    case 'int':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->operator.' '.$right->getCode().')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                                    case 'double':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression('int', '('.$variableLeft->getName().' '.$this->operator.' zephir_is_true('.$variableCode.'))', $expression);

                                    default:
                                        throw new CompilerException("Cannot compare variable('int') with variable('".$variableRight->getType()."')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable('int') with '".$right->getType()."'", $expression);
                        }
                        break;

                    case 'bool':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->operator.' '.$right->getCode().')', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->bitOperator.' '.$right->getBooleanCode().')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->bitOperator.' '.$variableRight->getName().')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->operator.' zephir_is_true('.$variableCode.'))', $expression);

                                    default:
                                        throw new CompilerException("Cannot compare variable('int') with variable('".$variableRight->getType()."')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable('int') with '".$right->getType()."'", $expression);
                        }
                        break;

                    case 'double':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', $variableLeft->getName().' '.$this->operator.' '.$right->getCode(), $expression);

                            case 'double':
                                return new CompiledExpression('bool', $variableLeft->getName().' '.$this->operator.' '.$right->getCode(), $expression);

                            case 'bool':
                                return new CompiledExpression('bool', $variableLeft->getName().' '.$this->bitOperator.''.$right->getBooleanCode(), $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                        return new CompiledExpression('bool', $variableLeft->getName().' '.$this->operator.'  (double) '.$variableRight->getName(), $expression);

                                    case 'double':
                                        return new CompiledExpression('bool', $variableLeft->getName().' '.$this->operator.'  '.$variableRight->getName(), $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', $variableLeft->getName().' '.$this->bitOperator.''.$variableRight->getName(), $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression('bool', $variableLeft->getName().' '.$this->operator.' zephir_is_true('.$variableCode.')', $expression);

                                    default:
                                        throw new CompilerException("Cannot compare variable('double') with variable('".$variableRight->getType()."')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable('int') with '".$right->getType()."'", $expression);
                        }
                        break;

                    case 'string':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', '('.$variableLeft->getName().' && Z_STRLEN_P('.$variableLeft->getName().')) '.$this->operator.' '.$right->getCode(), $expression);

                            case 'double':
                                return new CompiledExpression('bool', '('.$variableLeft->getName().' && Z_STRLEN_P('.$variableLeft->getName().')) '.$this->operator.' '.$right->getCode(), $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '('.$variableLeft->getName().' && Z_STRLEN_P('.$variableLeft->getName().')) '.$this->bitOperator.''.$right->getBooleanCode(), $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' && Z_STRLEN_P('.$variableLeft->getName().')) '.$this->operator.' '.$variableRight->getName(), $expression);

                                    case 'double':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' && Z_STRLEN_P('.$variableLeft->getName().')) '.$this->operator.'  '.$variableRight->getName(), $expression);

                                    case 'string':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' && Z_STRLEN_P('.$variableLeft->getName().')) '.$this->operator.' ('.$variableRight->getName().' && Z_STRLEN_P('.$variableRight->getName().'))', $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' && Z_STRLEN_P('.$variableLeft->getName().')) '.$this->bitOperator.' '.$variableRight->getName(), $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' && Z_STRLEN_P('.$variableLeft->getName().')) '.' '.$this->operator.' zephir_is_true('.$variableCode.')', $expression);

                                    default:
                                        throw new CompilerException("Cannot compare variable('double') with variable('".$variableRight->getType()."')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable('int') with '".$right->getType()."'", $expression);
                        }
                        break;

                    case 'variable':
                        $variableLeftCode = $compilationContext->backend->getVariableCode($variableLeft);
                        switch ($right->getType()) {
                            /* a && 1 */
                            case 'int':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                $op = $this->operator;
                                $op1 = $variableLeftCode;
                                $op2 = $right->getCode();
                                $compilationContext->headersManager->add('kernel/operators');

                                return new CompiledExpression('bool', 'zephir_is_true('.$op1.') '.$op.' '.$op2, $expression);

                            /* a && 1 */
                            case 'bool':
                                $compilationContext->headersManager->add('kernel/operators');
                                $op = $this->operator;
                                $op1 = $variableLeftCode;
                                $op2 = $right->getCode();
                                $compilationContext->headersManager->add('kernel/operators');

                                return new CompiledExpression('bool', 'zephir_is_true('.$op1.') '.$op.' '.$op2, $expression);

                            /* a(var) && a(x) */
                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->resolve(null, $compilationContext), $compilationContext, $expression);
                                $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);
                                switch ($variableRight->getType()) {
                                    /* a(var) && a(int) */
                                    case 'int':
                                        $compilationContext->headersManager->add('kernel/operators');

                                        return new CompiledExpression('bool', 'zephir_is_true('.$variableLeftCode.') '.$this->operator.' '.$variableRightCode, $expression);

                                    /* a(var) && a(bool) */
                                    case 'bool':
                                        $compilationContext->headersManager->add('kernel/operators');

                                        return new CompiledExpression('bool', 'zephir_is_true('.$variableLeftCode.') '.$this->operator.' '.$variableRightCode, $expression);

                                    /* a(var) && a(var) */
                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $op1 = $variableLeftCode;
                                        $op2 = $variableRightCode;

                                        $expected = $this->getExpected($compilationContext, $expression);
                                        if ($expected->isLocalOnly()) {
                                            $compilationContext->codePrinter->output('add_function(&'.$expected->getName().', '.$op1.', '.$op2.');');
                                        } else {
                                            $compilationContext->codePrinter->output('add_function('.$expected->getName().', '.$op1.', '.$op2.');');
                                        }

                                        return new CompiledExpression('variable', $expected->getName(), $expression);

                                    default:
                                        throw new CompilerException("Cannot compare 'variable' with variable ('".$variableRight->getType()."')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare 'variable' with '".$right->getType()."'", $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown '".$variableLeft->getType()."'", $expression);
                }
                break;

            default:
                throw new CompilerException('Unsupported type: '.$left->getType(), $expression);
        }
    }
}
