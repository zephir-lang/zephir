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

namespace Zephir\Operators\Arithmetical;

use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\Compiler\CompilerException;
use Zephir\CompiledExpression;

/**
 * DivOperator
 *
 * Generates an arithmetical operation according to the operands
 */
class DivOperator extends ArithmeticalBaseOperator
{
    protected $_operator = '/';

    protected $_bitOperator = '-';

    protected $_zvalOperator = 'div_function';

    /**
     * Compiles the arithmetical division operation
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new \Exception("Missing left part of the expression");
        }

        if (!isset($expression['right'])) {
            throw new \Exception("Missing right part of the expression");
        }

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
                        return new CompiledExpression('double', 'zephir_safe_div_long_long(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                    case 'double':
                        return new CompiledExpression('double', 'zephir_safe_div_long_double((double) ' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                    case 'bool':
                        return new CompiledExpression('bool', '(' . $left->getCode() . ' - ' . $right->getBooleanCode() . ')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'bool':
                                return new CompiledExpression('double', 'zephir_safe_div_long_long(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                            case 'double':
                                return new CompiledExpression('double', 'zephir_safe_div_long_double(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                            case 'variable':
                                $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);
                                return new CompiledExpression('double', 'zephir_safe_div_long_zval(' . $left->getCode() . ', ' . $variableRightCode . ' TSRMLS_CC)', $expression);

                            default:
                                throw new CompilerException("Cannot operate variable('int') with variable('" . $variableRight->getType() . "')", $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot operate 'int' with '" . $right->getType() . "'", $expression);
                }
                break;

            case 'bool':
                switch ($right->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'double':
                        return new CompiledExpression('long', '(' . $left->getBooleanCode() . ' - ' . $right->getCode() . ')', $expression);

                    case 'bool':
                        return new CompiledExpression('bool', '(' . $left->getBooleanCode() . ' ' . $this->_bitOperator . ' ' . $right->getBooleanCode() . ')', $expression);

                    default:
                        throw new CompilerException("Cannot operate 'bool' with '" . $right->getType() . "'", $expression);
                }
                break;

            case 'double':
                switch ($right->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        return new CompiledExpression('double', 'zephir_safe_div_double_long(' . $left->getCode() . ', (double) (' . $right->getCode() . ') TSRMLS_CC)', $expression);

                    case 'double':
                        return new CompiledExpression('double', 'zephir_safe_div_double_long(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                    case 'bool':
                        return new CompiledExpression('double', 'zephir_safe_div_double_long(' . $left->getCode() . ', ' . $right->getBooleanCode() . ' TSRMLS_CC)', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'bool':
                                return new CompiledExpression('double', 'zephir_safe_div_double_long(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                            case 'double':
                                return new CompiledExpression('double', 'zephir_safe_div_double_double(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                            case 'variable':
                                $symbolRight = $compilationContext->backend->getVariableCode($variableRight);
                                return new CompiledExpression('double', 'zephir_safe_div_double_zval(' . $left->getCode() . ', ' . $symbolRight . ' TSRMLS_CC)', $expression);
                                break;

                            default:
                                throw new CompilerException("Cannot operate variable('double') with variable('" . $variableRight->getType() . "')", $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot operate 'double' with '" . $right->getType() . "'", $expression);
                }
                break;

            case 'string':
            case 'array':
                switch ($right->getType()) {
                    default:
                        throw new CompilerException("Operation is not supported between " . $right->getType(), $expression);
                }
                break;

            case 'variable':
                $variableLeft = $compilationContext->symbolTable->getVariableForRead($left->resolve(null, $compilationContext), $compilationContext, $expression);
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
                                return new CompiledExpression('double', 'zephir_safe_div_long_long(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                            case 'double':
                                return new CompiledExpression('double', 'zephir_safe_div_long_double(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'bool':
                                        return new CompiledExpression('double', 'zephir_safe_div_long_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'double':
                                        return new CompiledExpression('double', 'zephir_safe_div_long_double(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableRight->isLocalOnly()) {
                                            return new CompiledExpression('double', 'zephir_safe_div_long_zval(' . $variableLeft->getName() . ', &' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        } else {
                                            $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);
                                            return new CompiledExpression('double', 'zephir_safe_div_long_zval(' . $variableLeft->getName() . ', ' . $variableRightCode . ' TSRMLS_CC)', $expression);
                                        }
                                        break;

                                    default:
                                        throw new CompilerException("Cannot operate variable('int') with variable('" . $variableRight->getType() . "')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate variable('int') with '" . $right->getType() . "'", $expression);
                        }
                        break;

                    case 'bool':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression('bool', '(' . $left->getCode() . ', ' . $right->getCode() . ')', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_bitOperator . ' ' . $right->getBooleanCode() . ')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression('double', 'zephir_safe_div_long_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'double':
                                        return new CompiledExpression('double', 'zephir_safe_div_long_double(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'bool':
                                        return new CompiledExpression('double', 'zephir_safe_div_long_long(' . $variableLeft->getName() . ' ' . $this->_bitOperator . ' ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableRight->isLocalOnly()) {
                                            return new CompiledExpression('double', 'zephir_safe_div_long_zval(' . $variableLeft->getName() . ', &' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        } else {
                                            $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);
                                            return new CompiledExpression('double', 'zephir_safe_div_long_zval(' . $variableLeft->getName() . ', ' . $variableRightCode . ' TSRMLS_CC)', $expression);
                                        }
                                        break;

                                    default:
                                        throw new CompilerException("Cannot operate variable('int') with variable('" . $variableRight->getType() . "')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate variable('int') with '" . $right->getType() . "'", $expression);
                        }
                        break;

                    case 'double':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                return new CompiledExpression('double', 'zephir_safe_div_double_long(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_bitOperator . ' ' . $right->getBooleanCode() . ')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression('double', 'zephir_safe_div_double_long(' . $variableLeft->getName() . ',  (double) ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'double':
                                        return new CompiledExpression('double', 'zephir_safe_div_double_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_bitOperator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $symbolRight = $compilationContext->backend->getVariableCode($variableRight);
                                        return new CompiledExpression('double', 'zephir_safe_div_double_zval(' . $variableLeft->getName() . ', ' . $symbolRight . ' TSRMLS_CC)', $expression);

                                    default:
                                        throw new CompilerException("Cannot operate variable('double') with variable('" . $variableRight->getType() . "')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate variable('int') with '" . $right->getType() . "'", $expression);
                        }
                        break;

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
                                return new CompiledExpression('double', 'zephir_safe_div_zval_long(' . $op1 . ', ' . $op2 . ' TSRMLS_CC)', $expression);

                            case 'double':
                                $op2 = $right->getCode();
                                return new CompiledExpression('double', 'zephir_safe_div_zval_double(' . $op1 . ', ' . $op2 . ' TSRMLS_CC)', $expression);

                            /* a(var) + a(x) */
                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->resolve(null, $compilationContext), $compilationContext, $expression);
                                switch ($variableRight->getType()) {
                                    /* a(var) + a(int) */
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression('double', 'zephir_safe_div_zval_long(' . $op1 . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'double':
                                        return new CompiledExpression('double', 'zephir_safe_div_zval_double(' . $op1 . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    /* a(var) + a(bool) */
                                    case 'bool':
                                        return new CompiledExpression('int', 'zephir_safe_div_zval_long(' . $op1 . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    /* a(var) + a(var) */
                                    case 'variable':
                                        $op2 = $compilationContext->backend->getVariableCode($variableRight);

                                        $expected = $this->getExpected($compilationContext, $expression);
                                        $expectedCode = $compilationContext->backend->getVariableCode($expected);
                                        $compilationContext->codePrinter->output($this->_zvalOperator . '(' . $expectedCode . ', ' . $op1 . ', ' . $op2 . ' TSRMLS_CC);');

                                        if ($variableLeft->isTemporal()) {
                                            $variableLeft->setIdle(true);
                                        }
                                        if ($variableRight->isTemporal()) {
                                            $variableRight->setIdle(true);
                                        }

                                        return new CompiledExpression('variable', $expected->getName(), $expression);

                                    default:
                                        throw new CompilerException("Cannot operate 'variable' with variable ('" . $variableRight->getType() . "')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate 'variable' with '" . $right->getType() . "'", $expression);
                        }
                        break;

                    case 'variable':
                        $op1 = $compilationContext->backend->getVariableCode($variableLeft);
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                $op2 = $right->getCode();
                                return new CompiledExpression('double', 'zephir_safe_div_zval_long(' . $op1 . ', ' . $op2 . ' TSRMLS_CC)', $expression);

                            case 'double':
                                $op2 = $right->getCode();
                                return new CompiledExpression('double', 'zephir_safe_div_zval_double(' . $op1 . ', ' . $op2 . ' TSRMLS_CC)', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->resolve(null, $compilationContext), $compilationContext, $expression);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'bool':
                                        return new CompiledExpression('double', 'zephir_safe_div_zval_long(' . $op1 . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'double':
                                        return new CompiledExpression('double', 'zephir_safe_div_zval_double(' . $op1 . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'variable':
                                        $variableRight = $compilationContext->symbolTable->getVariableForRead($variableRight->getCode(), $compilationContext, $expression);
                                        $op2 = $compilationContext->backend->getVariableCode($variableRight);
                                        switch ($variableRight->getType()) {
                                            case 'int':
                                                return new CompiledExpression('double', 'zephir_safe_div_zval_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                            case 'double':
                                                return new CompiledExpression('double', 'zephir_safe_div_zval_double(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                            case 'bool':
                                                return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_bitOperator . ' ' . $variableRight->getName(), $expression);

                                            case 'variable':
                                                $expected = $this->getExpected($compilationContext, $expression);
                                                $expectedCode = $compilationContext->backend->getVariableCode($expected);
                                                $compilationContext->codePrinter->output($this->_zvalOperator . '(' . $expectedCode . ', ' . $op1 . ', ' . $op2 . ');');
                                                return new CompiledExpression('variable', $expected->getName(), $expression);

                                            default:
                                                throw new CompilerException("Cannot operate variable('double') with variable('" . $variableRight->getType() . "')", $expression);
                                        }
                                        break;

                                    default:
                                        throw new CompilerException("Cannot operate 'variable' with variable ('" . $variableRight->getType() . "')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate 'variable' with '" . $right->getType() . "'", $expression);
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
