<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Operators\Arithmetical;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompiledExpression;

/**
 * ModOperator
 *
 * Generates an arithmetical operation according to the operands
 */
class ModOperator extends ArithmeticalBaseOperator
{

    protected $_operator = '%';

    protected $_bitOperator = '-';

    protected $_zvalOperator = 'mod_function';

    /**
     * Compiles the arithmetical modulus operation
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
                        return new CompiledExpression('double', 'zephir_safe_mod_long_long(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                    case 'double':
                        return new CompiledExpression('double', 'zephir_safe_mod_long_double((double) ' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                    case 'bool':
                        return new CompiledExpression('bool', '(' . $left->getCode() . ' - ' . $right->getBooleanCode() . ')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {

                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression('double', 'zephir_safe_mod_long_long(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                            case 'bool':
                                return new CompiledExpression('double', 'zephir_safe_mod_long_long(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                            case 'double':
                                return new CompiledExpression('double', 'zephir_safe_mod_long_double(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                            case 'variable':
                                if ($variableRight->isLocalOnly()) {
                                    return new CompiledExpression('double', 'zephir_safe_mod_long_zval(' . $left->getCode() . ', &' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                } else {
                                    return new CompiledExpression('double', 'zephir_safe_mod_long_zval(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                }
                                break;

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
                        return new CompiledExpression('double', 'zephir_safe_mod_double_long(' . $left->getCode() . ', (double) (' . $right->getCode() . ') TSRMLS_CC)', $expression);

                    case 'double':
                        return new CompiledExpression('double', 'zephir_safe_mod_double_long(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                    case 'bool':
                        return new CompiledExpression('double', 'zephir_safe_mod_double_long(' . $left->getCode() . ', ' . $right->getBooleanCode() . ' TSRMLS_CC)', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression('double', 'zephir_safe_mod_double_long(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                            case 'bool':
                                return new CompiledExpression('double', 'zephir_safe_mod_double_long(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                            case 'double':
                                return new CompiledExpression('double', 'zephir_safe_mod_double_double(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                if ($variableRight->isLocalOnly()) {
                                    return new CompiledExpression('double', 'zephir_safe_mod_double_zval(' . $left->getCode() . ', &' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                } else {
                                    return new CompiledExpression('double', 'zephir_safe_mod_double_zval(' . $left->getCode() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                }
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
                switch ($right->getType()) {
                    default:
                        throw new CompilerException("Operation is not supported between strings", $expression);
                }
                break;

            case 'array':
                switch ($right->getType()) {
                    default:
                        throw new CompilerException("Operation is not supported between arrays", $expression);
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
                                return new CompiledExpression('double', 'zephir_safe_mod_long_long(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                            case 'double':
                                return new CompiledExpression('double', 'zephir_safe_mod_long_double(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {

                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression('double', 'zephir_safe_mod_long_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'bool':
                                        return new CompiledExpression('double', 'zephir_safe_mod_long_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'double':
                                        return new CompiledExpression('double', 'zephir_safe_mod_long_double(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableRight->isLocalOnly()) {
                                            return new CompiledExpression('double', 'zephir_safe_mod_long_zval(' . $variableLeft->getName() . ', &' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        } else {
                                            return new CompiledExpression('double', 'zephir_safe_mod_long_zval(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
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
                                        return new CompiledExpression('double', 'zephir_safe_mod_long_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'double':
                                        return new CompiledExpression('double', 'zephir_safe_mod_long_double(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'bool':
                                        return new CompiledExpression('double', 'zephir_safe_mod_long_long(' . $variableLeft->getName() . ' ' . $this->_bitOperator . ' ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableRight->isLocalOnly()) {
                                            return new CompiledExpression('double', 'zephir_safe_mod_long_zval(' . $variableLeft->getName() . ', &' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        } else {
                                            return new CompiledExpression('double', 'zephir_safe_mod_long_zval(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
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
                                return new CompiledExpression('double', 'zephir_safe_mod_double_long(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                            case 'double':
                                return new CompiledExpression('double', 'zephir_safe_mod_double_long(' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC)', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '(' . $left->getCode() . ' ' . $this->_bitOperator . ' ' . $right->getBooleanCode() . ')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {

                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression('double', 'zephir_safe_mod_double_long(' . $variableLeft->getName() . ',  (double) ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'double':
                                        return new CompiledExpression('double', 'zephir_safe_mod_double_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', '(' . $variableLeft->getName() . ' ' . $this->_bitOperator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableRight->isLocalOnly()) {
                                            return new CompiledExpression('double', 'zephir_safe_mod_double_zval(' . $variableLeft->getName() . ', &' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        } else {
                                            return new CompiledExpression('double', 'zephir_safe_mod_double_zval(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        }
                                        break;

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
                        switch ($right->getType()) {

                            /* a + 1 */
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                $compilationContext->headersManager->add('kernel/operators');
                                if ($variableLeft->isLocalOnly()) {
                                    $op1 = '&' . $variableLeft->getName();
                                } else {
                                    $op1 = $variableLeft->getName();
                                }
                                $op2 = $right->getCode();
                                if ($right->getType() == 'double') {
                                    return new CompiledExpression('double', 'zephir_safe_mod_zval_long(' . $op1 . ', ' . $op2 . ' TSRMLS_CC)', $expression);
                                } else {
                                    return new CompiledExpression('double', 'zephir_safe_mod_zval_long(' . $op1 . ', ' . $op2 . ' TSRMLS_CC)', $expression);
                                }
                                break;

                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                if ($variableLeft->isLocalOnly()) {
                                    $op1 = '&' . $variableLeft->getName();
                                } else {
                                    $op1 = $variableLeft->getName();
                                }
                                $op2 = $right->getCode();
                                if ($right->getType() == 'double') {
                                    return new CompiledExpression('double', 'zephir_safe_mod_zval_double(' . $op1 . ', ' . $op2 . ' TSRMLS_CC)', $expression);
                                } else {
                                    return new CompiledExpression('double', 'zephir_safe_mod_zval_double(' . $op1 . ', ' . $op2 . ' TSRMLS_CC)', $expression);
                                }
                                break;

                            /* a(var) + a(x) */
                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->resolve(null, $compilationContext), $compilationContext, $expression);
                                switch ($variableRight->getType()) {

                                    /* a(var) + a(int) */
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableLeft->isLocalOnly()) {
                                            return new CompiledExpression('double', 'zephir_safe_mod_zval_long(&' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        } else {
                                            return new CompiledExpression('double', 'zephir_safe_mod_zval_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        }
                                        break;

                                    /* a(var) + a(bool) */
                                    case 'bool':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableLeft->isLocalOnly()) {
                                            return new CompiledExpression('int', 'zephir_safe_mod_zval_long(&' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        } else {
                                            return new CompiledExpression('int', 'zephir_safe_mod_zval_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        }
                                        break;

                                    /* a(var) + a(var) */
                                    case 'variable':

                                        $compilationContext->headersManager->add('kernel/operators');

                                        if ($variableLeft->isLocalOnly()) {
                                            $op1 = '&' . $variableLeft->getName();
                                        } else {
                                            $op1 = $variableLeft->getName();
                                        }

                                        if ($variableRight->isLocalOnly()) {
                                            $op2 = '&' . $variableRight->getName();
                                        } else {
                                            $op2 = $variableRight->getName();
                                        }

                                        $expected = $this->getExpected($compilationContext, $expression);
                                        if ($expected->isLocalOnly()) {
                                            $compilationContext->codePrinter->output($this->_zvalOperator . '(&' . $expected->getName() . ', ' . $op1 . ', ' . $op2 . ' TSRMLS_CC);');
                                        } else {
                                            $compilationContext->codePrinter->output($this->_zvalOperator . '(' . $expected->getName() . ', ' . $op1 . ', ' . $op2 . ' TSRMLS_CC);');
                                        }

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
                        switch ($right->getType()) {

                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                $compilationContext->headersManager->add('kernel/operators');
                                if ($variableLeft->isLocalOnly()) {
                                    $op1 = '&' . $variableLeft->getName();
                                } else {
                                    $op1 = $variableLeft->getName();
                                }
                                $op2 = $right->getCode();
                                return new CompiledExpression('double', 'zephir_safe_mod_zval_long(' . $op1 . ', ' . $op2 . ' TSRMLS_CC)', $expression);

                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                if ($variableLeft->isLocalOnly()) {
                                    $op1 = '&' . $variableLeft->getName();
                                } else {
                                    $op1 = $variableLeft->getName();
                                }
                                $op2 = $right->getCode();
                                return new CompiledExpression('double', 'zephir_safe_mod_zval_double(' . $op1 . ', ' . $op2 . ' TSRMLS_CC)', $expression);

                            case 'variable':

                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->resolve(null, $compilationContext), $compilationContext, $expression);

                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableLeft->isLocalOnly()) {
                                            return new CompiledExpression('double', 'zephir_safe_mod_zval_long(&' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        } else {
                                            return new CompiledExpression('double', 'zephir_safe_mod_zval_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        }
                                        break;

                                    case 'bool':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableLeft->isLocalOnly()) {
                                            return new CompiledExpression('double', 'zephir_safe_mod_zval_long(&' . $variableLeft->getName() . '), ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        } else {
                                            return new CompiledExpression('double', 'zephir_safe_mod_zval_long(' . $variableLeft->getName() . '), ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);
                                        }
                                        break;

                                    case 'variable':
                                        $variableRight = $compilationContext->symbolTable->getVariableForRead($variableRight->getCode(), $compilationContext, $expression);
                                        switch ($variableRight->getType()) {

                                            case 'int':
                                                return new CompiledExpression('double', 'zephir_safe_mod_zval_long(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                            case 'double':
                                                return new CompiledExpression('double', 'zephir_safe_mod_zval_double(' . $variableLeft->getName() . ', ' . $variableRight->getName() . ' TSRMLS_CC)', $expression);

                                            case 'bool':
                                                return new CompiledExpression('bool', $variableLeft->getName() . ' ' . $this->_bitOperator . ' ' . $variableRight->getName(), $expression);

                                            case 'variable':
                                                $compilationContext->headersManager->add('kernel/operators');

                                                if ($variableLeft->isLocalOnly()) {
                                                    $op1 = '&' . $variableLeft->getName();
                                                } else {
                                                    $op1 = $variableLeft->getName();
                                                }
                                                if ($variableRight->isLocalOnly()) {
                                                    $op2 = '&' . $variableRight->getName();
                                                } else {
                                                    $op2 = $variableRight->getName();
                                                }

                                                $expected = $this->getExpected($compilationContext, $expression);
                                                $compilationContext->codePrinter->output($this->_zvalOperator . '(' . $expected->getName() . ', ' . $op1 . ', ' . $op2 . ');');
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
