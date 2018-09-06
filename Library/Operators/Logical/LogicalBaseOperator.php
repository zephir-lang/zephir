<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Operators\Logical;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\Expression;
use Zephir\CompiledExpression;

/**
 * LogicalBaseOperator
 *
 * This is the base operator for logical operators
 */
class LogicalBaseOperator extends BaseOperator
{
    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Missing left part of the expression", $expression);
        }

        if (!isset($expression['right'])) {
            throw new CompilerException("Missing right part of the expression", $expression);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->_readOnly);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly($this->_readOnly);
        $right = $rightExpr->compile($compilationContext);

        switch ($left->getType()) {
            case 'int':
                switch ($right->getType()) {
                    case 'int':
                        return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                    case 'double':
                        return new CompiledExpression('double', '((double) ' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                    case 'bool':
                        return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode() . ')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'double':
                                return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $variableCode = $compilationContext->backend->getVariableCode($variableRight);
                                return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' zephir_is_true(' . $variableCode . '))', $expression);

                            default:
                                throw new CompilerException("Cannot compare variable('int') with variable('" . $variableRight->getType() . "')", $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot compare 'int' with '" . $right->getType() . "'", $expression);
                }
                break;

            case 'bool':
                switch ($right->getType()) {
                    case 'int':
                    case 'double':
                        return new CompiledExpression('bool', '(' . $left->getBooleanCode() . ' ' . $this->_bitOperator . ' ((' . $right->getCode() . ') ? 1 : 0))', $expression);

                    case 'bool':
                        return new CompiledExpression('bool', '(' . $left->getBooleanCode() . ' ' . $this->_bitOperator . ' ' . $right->getBooleanCode() . ')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', '(' . $left->getBooleanCode() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '(' . $left->getBooleanCode() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'double':
                                return new CompiledExpression('bool', '(' . $left->getBooleanCode() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'variable':
                                $variableCode = $compilationContext->backend->getVariableCode($variableRight);
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', '(' . $left->getBooleanCode() . ' ' . $this->_operator . ' zephir_is_true(' . $variableCode . '))', $expression);

                            default:
                                throw new CompilerException("Cannot add variable('int') with variable('" . $variableRight->getType() . "')", $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot compare 'bool' with '" . $right->getType() . "'", $expression);
                }
                break;
            case 'double':
                switch ($right->getType()) {
                    case 'int':
                        return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                    case 'double':
                        return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                    case 'bool':
                        return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode() . ')', $expression);

                    default:
                        throw new CompilerException("Cannot compare 'double' with '" . $right->getType() . "'", $expression);
                }
                break;

            case 'string':
                switch ($right->getType()) {
                    default:
                        throw new CompilerException("Operation is not supported between strings", $expression);
                }
                break;

            case 'variable':
                $variableLeft = $compilationContext->symbolTable->getVariableForRead($left->resolve(null, $compilationContext), $compilationContext, $expression);
                switch ($variableLeft->getType()) {
                    case 'int':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'double':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);
                                        return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' zephir_is_true(' . $variableCode . '))', $expression);

                                    default:
                                        throw new CompilerException("Cannot compare variable('int') with variable('" . $variableRight->getType() . "')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable('int') with '" . $right->getType() . "'", $expression);
                        }
                        break;

                    case 'bool':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_bitOperator . ' ' . $right->getBooleanCode() . ')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_bitOperator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' zephir_is_true(' . $variableCode . '))', $expression);

                                    default:
                                        throw new CompilerException("Cannot compare variable('int') with variable('" . $variableRight->getType() . "')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable('int') with '" . $right->getType() . "'", $expression);
                        }
                        break;

                    case 'double':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);

                            case 'double':
                                return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);

                            case 'bool':
                                return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_bitOperator . '' . $right->getBooleanCode(), $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                        return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_operator . '  (double) ' . $variableRight->getName(), $expression);

                                    case 'double':
                                        return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_operator . '  ' . $variableRight->getName(), $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_bitOperator . '' . $variableRight->getName(), $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);
                                        return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_operator . ' zephir_is_true(' . $variableCode . ')', $expression);

                                    default:
                                        throw new CompilerException("Cannot compare variable('double') with variable('" . $variableRight->getType() . "')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable('int') with '" . $right->getType() . "'", $expression);
                        }
                        break;

                    case 'string':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName() . ')) ' . $this->_operator . ' ' . $right->getCode(), $expression);

                            case 'double':
                                return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName() . ')) ' . $this->_operator . ' ' . $right->getCode(), $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName() . ')) ' . $this->_bitOperator . '' . $right->getBooleanCode(), $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName() . ')) ' . $this->_operator . ' ' . $variableRight->getName(), $expression);

                                    case 'double':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName() . ')) ' . $this->_operator . '  ' . $variableRight->getName(), $expression);

                                    case 'string':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName() . ')) ' . $this->_operator . ' (' . $variableRight->getName() . ' && Z_STRLEN_P(' . $variableRight->getName() . '))', $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName() . ')) ' . $this->_bitOperator . ' ' . $variableRight->getName(), $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableCode = $compilationContext->backend->getVariableCode($variableRight);
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' && Z_STRLEN_P(' . $variableLeft->getName() . ')) ' . ' ' . $this->_operator . ' zephir_is_true(' . $variableCode . ')', $expression);

                                    default:
                                        throw new CompilerException("Cannot compare variable('double') with variable('" . $variableRight->getType() . "')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable('int') with '" . $right->getType() . "'", $expression);
                        }
                        break;

                    case 'variable':
                        $variableLeftCode = $compilationContext->backend->getVariableCode($variableLeft);
                        switch ($right->getType()) {
                            /* a && 1 */
                            case 'int':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                $op = $this->_operator;
                                $op1 = $variableLeftCode;
                                $op2 = $right->getCode();
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', 'zephir_is_true(' . $op1 . ') ' . $op . ' ' . $op2, $expression);

                            /* a && 1 */
                            case 'bool':
                                $compilationContext->headersManager->add('kernel/operators');
                                $op = $this->_operator;
                                $op1 = $variableLeftCode;
                                $op2 = $right->getCode();
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', 'zephir_is_true(' . $op1 . ') ' . $op . ' ' . $op2, $expression);

                            /* a(var) && a(x) */
                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->resolve(null, $compilationContext), $compilationContext, $expression);
                                $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);
                                switch ($variableRight->getType()) {
                                    /* a(var) && a(int) */
                                    case 'int':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        return new CompiledExpression('bool', 'zephir_is_true(' . $variableLeftCode . ') ' . $this->_operator . ' ' . $variableRightCode, $expression);

                                    /* a(var) && a(bool) */
                                    case 'bool':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        return new CompiledExpression('bool', 'zephir_is_true(' . $variableLeftCode . ') ' . $this->_operator . ' ' . $variableRightCode, $expression);

                                    /* a(var) && a(var) */
                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $op1 = $variableLeftCode;
                                        $op2 = $variableRightCode;

                                        $expected = $this->getExpected($compilationContext, $expression);
                                        if ($expected->isLocalOnly()) {
                                            $compilationContext->codePrinter->output('add_function(&' . $expected->getName() . ', ' . $op1 . ', ' . $op2 . ' TSRMLS_CC);');
                                        } else {
                                            $compilationContext->codePrinter->output('add_function(' . $expected->getName() . ', ' . $op1 . ', ' . $op2 . ' TSRMLS_CC);');
                                        }
                                        return new CompiledExpression('variable', $expected->getName(), $expression);

                                    default:
                                        throw new CompilerException("Cannot compare 'variable' with variable ('" . $variableRight->getType() . "')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare 'variable' with '" . $right->getType() . "'", $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown '" . $variableLeft->getType() . "'", $expression);
                }
                break;

            default:
                throw new CompilerException("Unsupported type: " . $left->getType(), $expression);
        }
    }
}
