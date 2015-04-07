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
 | Authors: Rack Lin <racklin@gmail.com>                                    |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Operators\Bitwise;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompiledExpression;

/**
 * BaseOperator
 *
 * This is the base operator for commutative, associative and distributive
 * arithmetic operators
 */
class BitwiseBaseOperator extends BaseOperator
{

    protected $_literalOnly = true;

    /**
     * This tries to perform arithmetical operations
     * Probably gcc/clang will optimize them without this optimization
     * @see http://en.wikipedia.org/wiki/Constant_folding
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return bool|CompiledExpression
     */
    public function optimizeConstantFolding(array $expression, CompilationContext $compilationContext)
    {
        if (!$compilationContext->config->get('constant-folding', 'optimizations')) {
            return false;
        }

        switch ($expression['left']['type']) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
                //continue to next switch
                break;
            default:
                return false;
                break;
        }

        switch ($expression['right']['type']) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
                //continue to operator switch
                break;
            default:
                return false;
                break;
        }

        /**
         * Return value will be always int
         */
        switch ($this->_operator) {
            case '&':
                return new CompiledExpression('int', $expression['left']['value'] & $expression['right']['value'], $expression);

            case '|':
                return new CompiledExpression('int', $expression['left']['value'] | $expression['right']['value'], $expression);

            case '^':
                return new CompiledExpression('int', $expression['left']['value'] ^ $expression['right']['value'], $expression);

            case '<<':
                return new CompiledExpression('int', $expression['left']['value'] << $expression['right']['value'], $expression);

            case '>>':
                return new CompiledExpression('int', $expression['left']['value'] >> $expression['right']['value'], $expression);
        }

        return false;
    }

    /**
     * @param array $expression
     * @param CompilationContext $compilationContext
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Missing left part of the expression", $expression);
        }

        if (!isset($expression['right'])) {
            throw new CompilerException("Missing right part of the expression", $expression);
        }

        /**
         * Check for constant folding optimizations
         */
        switch ($this->_operator) {
            case '&':
            case '|':
            case '^':
            case '<<':
            case '>>':
                $optimized = $this->optimizeConstantFolding($expression, $compilationContext);
                if (is_object($optimized)) {
                    return $optimized;
                }
                break;
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
                        return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                    case 'double':
                        return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' (int) (' . $right->getCode() . '))', $expression);

                    case 'bool':
                        return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode() . ')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {

                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'bool':
                                return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'double':
                                return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' (int) (' . $variableRight->getName() . '))', $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                if ($variableRight->isLocalOnly()) {
                                    return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' zephir_get_numberval(&' . $variableRight->getName() . '))', $expression);
                                } else {
                                    return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' zephir_get_numberval(' . $variableRight->getName() . '))', $expression);
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
                        return new CompiledExpression('int', '(' . $left->getBooleanCode() . ' ' . $this->_bitOperator . '((' . $right->getCode() . ') ? 1 : 0))', $expression);

                    case 'bool':
                        return new CompiledExpression('int', '(' . $left->getBooleanCode() . ' ' . $this->_bitOperator . ' ' . $right->getBooleanCode() . ')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value'], $compilationContext, $expression);
                        switch ($variableRight->getType()) {

                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression('int', '((int) (' . $left->getBooleanCode() . ') ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'bool':
                                return new CompiledExpression('int', '((int) (' . $left->getBooleanCode() . ') ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'double':
                                return new CompiledExpression('int', '((int) (' . $left->getBooleanCode() . ') ' . $this->_operator . ' (int) (' . $variableRight->getName() . '))', $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                if ($variableRight->isLocalOnly()) {
                                    return new CompiledExpression('int', '((int) (' . $left->getBooleanCode() . ') ' . $this->_operator . ' zephir_get_numberval(&' . $variableRight->getName() . '))', $expression);
                                } else {
                                    return new CompiledExpression('int', '((int) (' . $left->getBooleanCode() . ') ' . $this->_operator . ' zephir_get_numberval(' . $variableRight->getName() . '))', $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate ('bool') with variable('" . $variableRight->getType() . "')", $expression);
                        }
                        break;

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
                        return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                    case 'double':
                        return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_operator . ' (int) (' . $right->getCode() . '))', $expression);

                    case 'bool':
                        return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_operator . ' ' . $right->getBooleanCode() . ')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value'], $compilationContext, $expression);
                        switch ($variableRight->getType()) {

                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'bool':
                                return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                            case 'double':
                                return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_operator . ' (int) (' . $variableRight->getName() . '))', $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                if ($variableRight->isLocalOnly()) {
                                    return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_operator . ' zephir_get_numberval(&' . $variableRight->getName() . '))', $expression);
                                } else {
                                    return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_operator . ' zephir_get_numberval(' . $variableRight->getName() . '))', $expression);
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

            case 'variable':

                $variableLeft = $compilationContext->symbolTable->getVariableForRead($left->resolve(null, $compilationContext), $compilationContext, $expression);
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
                                return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {

                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'char':
                                    case 'uchar':
                                        return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'bool':
                                        return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'double':
                                        return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' (int) (' . $variableRight->getName() . '))', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableRight->isLocalOnly()) {
                                            return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' (int) (zephir_get_numberval(&' . $variableRight->getName() . ')))', $expression);
                                        } else {
                                            return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' (int) (zephir_get_numberval(' . $variableRight->getName() . ')))', $expression);
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
                                return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                            case 'bool':
                                return new CompiledExpression('int', '(' . $left->getCode() . ' ' . $this->_bitOperator . ' ' . $right->getBooleanCode() . ')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {

                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'double':
                                        return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' (int) (' . $variableRight->getName() . '))', $expression);

                                    case 'bool':
                                        return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_bitOperator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableRight->isLocalOnly()) {
                                            return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' zephir_get_numberval(&' . $variableRight->getName() . '))', $expression);
                                        } else {
                                            return new CompiledExpression('int', '(' . $variableLeft->getName() . ' ' . $this->_operator . ' zephir_get_numberval(' . $variableRight->getName() . '))', $expression);
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
                                return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                            case 'double':
                                return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_operator . ' (int) (' . $right->getCode() . '))', $expression);

                            case 'bool':
                                return new CompiledExpression('int', '((int) (' . $left->getCode() . ') ' . $this->_bitOperator . ' ' . $right->getBooleanCode() . ')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($expression['right']['value'], $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {

                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression('int', '((int) (' . $variableLeft->getName() . ') ' . $this->_operator . '  ' . $variableRight->getName() . ')', $expression);
                                    case 'double':
                                        return new CompiledExpression('int', '((int) (' . $variableLeft->getName() . ') ' . $this->_operator . ' (int) (' . $variableRight->getName() . '))', $expression);

                                    case 'bool':
                                        return new CompiledExpression('int', '((int) (' . $variableLeft->getName() . ') ' . $this->_bitOperator . ' ' . $variableRight->getName() . ')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableRight->isLocalOnly()) {
                                            return new CompiledExpression('int', '((int) (' . $variableLeft->getName() . ') ' . $this->_operator . ' (int) (zephir_get_numberval(&' . $variableRight->getName() . ')))', $expression);
                                        } else {
                                            return new CompiledExpression('int', '((int) (' . $variableLeft->getName() . ') ' . $this->_operator . ' (int) (zephir_get_numberval(' . $variableRight->getName() . ')))', $expression);
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

                    case 'variable':
                        switch ($right->getType()) {

                            /* a + 1 */
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                $op = $this->_operator;
                                if ($variableLeft->isLocalOnly()) {
                                    $op1 = '&' . $variableLeft->getName();
                                } else {
                                    $op1 = $variableLeft->getName();
                                }
                                $op2 = $right->getCode();
                                if ($right->getType() == 'double') {
                                    return new CompiledExpression('int', '((int) (zephir_get_numberval(' . $op1 . ')) ' . $op . ' (int) (' . $op2 . '))', $expression);
                                } else {
                                    return new CompiledExpression('int', '((int) (zephir_get_numberval(' . $op1 . ')) ' . $op . ' ' . $op2 . ')', $expression);
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
                                            return new CompiledExpression('int', '((int) (zephir_get_numberval(&' . $variableLeft->getName() . ')) ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);
                                        } else {
                                            return new CompiledExpression('int', '((int) (zephir_get_numberval(' . $variableLeft->getName() . ')) ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);
                                        }
                                        break;

                                    /* a(var) + a(bool) */
                                    case 'bool':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        if ($variableLeft->isLocalOnly()) {
                                            return new CompiledExpression('int', '((int) (zephir_get_numberval(&' . $variableLeft->getName() . ')) ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);
                                        } else {
                                            return new CompiledExpression('int', '((int) (zephir_get_numberval(' . $variableLeft->getName() . ')) ' . $this->_operator . ' ' . $variableRight->getName() . ')', $expression);
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

                    default:
                        throw new CompilerException("Unknown '" . $variableLeft->getType() . "'", $expression);
                }
                break;

            default:
                throw new CompilerException("Unsupported type: " . $left->getType(), $expression);
        }
    }
}
