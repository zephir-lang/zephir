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

namespace Zephir\Operators\Comparison;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompiledExpression;
use Zephir\Compiler\CompilerException;

/**
 * BaseOperator
 *
 * This is the base operator for comparison operators
 */
class ComparisonBaseOperator extends BaseOperator
{
    protected $_literalOnly = true;

    protected $_commutative = false;

    /**
     *
     *
     * @param $expr
     * @param CompilationContext $compilationContext
     * @return bool|CompiledExpression
     * @throws CompilerException
     */
    public function optimizeTypeOf($expr, CompilationContext $compilationContext)
    {
        if (!isset($expr['left'])) {
            return false;
        }

        if (!isset($expr['right']) && !isset($expr['right']['value'])) {
            return false;
        }

        if ($expr['left']['type'] == 'typeof') {
            if ($expr['right']['type'] != 'string') {
                $compilationContext->logger->warning("Possible invalid comparison for 'typeof' operator with non-string", "invalid-typeof-comparison", $expr['right']);
                return false;
            }

            if (isset($expr['type'])) {
                switch ($expr['type']) {
                    case 'identical':
                    case 'equals':
                        $operator = '==';
                        break;

                    case 'not-identical':
                    case 'not-equals':
                        $operator = '!=';
                        break;

                    default:
                        return false;
                }
            }

            $expression = new Expression($expr['left']['left']);
            $condition = $expression->compile($compilationContext);
            $variableVariable = $compilationContext->symbolTable->getVariableForRead($condition->getCode(), $compilationContext, $expr);

            if ($expr['right']['type'] != 'string') {
                throw new CompilerException('Right expression of typeof operator must be "string" type', $expr['right']);
            }

            $value = strtolower($expr['right']['value']);

            switch ($variableVariable->getType()) {
                case 'double':
                    switch ($value) {
                        case 'double':
                        case 'float':
                            $condition = '1 ' . $operator . ' 1';
                            break;

                        default:
                            $condition = '1 ' . $operator . ' 0';
                            break;
                    }
                    break;

                case 'int':
                case 'integer':
                case 'long':
                    switch ($value) {
                        case 'int':
                        case 'integer':
                        case 'long':
                            $condition = '1 ' . $operator . ' 1';
                            break;

                        default:
                            $condition = '1 ' . $operator . ' 0';
                            break;
                    }
                    break;

                case 'bool':
                    switch ($value) {
                        case 'bool':
                        case 'boolean':
                            $condition = '1 ' . $operator . ' 1';
                            break;

                        default:
                            $condition = '1 ' . $operator . ' 0';
                            break;
                    }
                    break;

                case 'array':
                    switch ($value) {
                        case 'array':
                            $condition = '1 ' . $operator . ' 1';
                            break;

                        default:
                            $condition = '1 ' . $operator . ' 0';
                            break;
                    }
                    break;

                case 'string':
                    switch ($value) {
                        case 'string':
                            $condition = '1 ' . $operator . ' 1';
                            break;

                        default:
                            $condition = '1 ' . $operator . ' 0';
                            break;
                    }
                    break;

                case 'variable':
                    $condition = $compilationContext->backend->getTypeofCondition($variableVariable, $operator, $value, $compilationContext);
                    break;

                default:
                    return false;
            }

            return new CompiledExpression('bool', $condition, $expr);
        }

        return false;
    }

    /**
     * Compile the expression
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $conditions = $this->optimizeTypeOf($expression, $compilationContext);
        if ($conditions !== false) {
            return $conditions;
        }

        if (!isset($expression['left'])) {
            throw new CompilerException("Missing left part of the expression", $expression);
        }

        if (!isset($expression['right'])) {
            throw new CompilerException("Missing right part of the expression", $expression);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly(true);
        $right = $rightExpr->compile($compilationContext);

        switch ($left->getType()) {
            case 'null':
                switch ($right->getType()) {
                    case 'null':
                        return new CompiledExpression('bool', '(0 ' . $this->_operator . ' 0)', $expression);

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        return new CompiledExpression('bool', '(0 ' . $this->_operator . ' ' . $right->getCode() . ')', $expression);

                    case 'char':
                    case 'uchar':
                        return new CompiledExpression('bool', '(\'\\0\' ' . $this->_operator . ' \'' . $right->getCode() . '\')', $expression);

                    case 'double':
                        return new CompiledExpression('bool', '(0 ' . $this->_operator . ' (int) ' . $right->getCode() . ')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', '0 ' . $this->_operator . ' ' . $variableRight->getName(), $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $condition = $compilationContext->backend->getTypeofCondition($variableRight, $this->_operator, 'null', $compilationContext);
                                return new CompiledExpression('bool', $condition, $expression);

                            default:
                                throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown type: " . $right->getType(), $expression);
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'double':
            case 'ulong':
            case 'char':
            case 'uchar':
                switch ($right->getType()) {
                    case 'null':
                        return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator, $expression);

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);

                    case 'char':
                    case 'uchar':
                        return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' \'' . $right->getCode() . '\'', $expression);

                    case 'double':
                        return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' (int) ' . $right->getCode(), $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $variableCode = $compilationContext->backend->getVariableCode($variableRight);
                                return new CompiledExpression('bool', $this->_zvalLongNegOperator . '(' . $variableCode . ', ' . $left->getCode() . ')', $expression);

                            default:
                                throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot compare " . $left->getType() . " with " . $right->getType(), $expression);
                }
                break;

            case 'bool':
                switch ($right->getType()) {
                    case 'null':
                        return new CompiledExpression('bool', $left->getBooleanCode() . ' ' . $this->_operator . ' 0', $expression);

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        return new CompiledExpression('bool', $left->getBooleanCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);

                    case 'char':
                    case 'uchar':
                        return new CompiledExpression('bool', $left->getBooleanCode() . ' ' . $this->_operator . ' \'' . $right->getCode() . '\'', $expression);

                    case 'double':
                        return new CompiledExpression('bool', $left->getBooleanCode() . ' ' . $this->_operator . ' (int) ' . $right->getCode(), $expression);

                    case 'bool':
                        return new CompiledExpression('bool', $left->getBooleanCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode(), $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', $left->getBooleanCode() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $boolOperator = $left->getBooleanCode() == '1' ? $this->_zvalBoolTrueOperator : $this->_zvalBoolFalseOperator;
                                $variableRight = $compilationContext->backend->getVariableCode($variableRight);
                                return new CompiledExpression('bool', $boolOperator . '(' . $variableRight . ')', $expression);
                            default:
                                throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot compare " . $left->getType() . " with " . $right->getType(), $expression);
                }
                break;

            case 'string':
                $variableLeft = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
                $variableLeftCode = $compilationContext->backend->getVariableCode($variableLeft);
                $compilationContext->backend->assignString($variableLeft, $left->getCode(), $compilationContext, true, false);
                switch ($right->getType()) {
                    case 'string':
                    case 'null':
                        $rightStr = $right->getType() == 'null' ? '' : $right->getCode();
                        $compilationContext->headersManager->add('kernel/operators');
                        return new CompiledExpression('bool', $this->_zvalStringOperator . '(' . $variableLeftCode . ', "' . $rightStr . '")', $expression['left']);
                        break;

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
                        switch ($variableRight->getType()) {
                            case 'string':
                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $variableRight = $compilationContext->backend->getVariableCode($variableRight);
                                return new CompiledExpression('bool', $this->_zvalOperator . '(' . $variableLeftCode . ', ' . $variableRight . ')', $expression);
                                break;

                            default:
                                throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown type: " . $right->getType(), $expression['left']);
                }
                break;

            case 'variable':
                $variable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['left']);
                $variableCode = $compilationContext->backend->getVariableCode($variable);
                switch ($variable->getType()) {
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
                                return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);

                            case 'char':
                            case 'uchar':
                                return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' \'' . $right->getCode() . '\'', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode(), $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'char':
                                    case 'uchar':
                                    case 'double':
                                        return new CompiledExpression('bool', $variable->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);
                                        $variableCode = $compilationContext->backend->getVariableCode($variable);
                                        return new CompiledExpression('bool', $this->_zvalLongNegOperator . '(' . $variableRightCode . ', ' . $variableCode . ')', $expression);
                                        break;

                                    default:
                                        throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable: " . $variable->getType() . " with: " . $right->getType(), $expression);
                        }
                        break;

                    case 'double':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression);

                            case 'bool':
                                return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode(), $expression);

                            case 'char':
                            case 'uchar':
                                return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' \'' . $right->getCode() . '\'', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'double':
                                        return new CompiledExpression('bool', $variable->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);
                                        $variableCode = $compilationContext->backend->getVariableCode($variable);
                                        return new CompiledExpression('bool', $this->_zvalDoubleNegOperator . '(' . $variableRightCode . ', ' . $variableCode . ')', $expression);

                                    default:
                                        throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable: " . $variable->getType() . " with: " . $right->getType(), $expression);
                        }
                        break;

                    case 'bool':
                        switch ($right->getType()) {
                            case 'int':
                                return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getCode(), $expression['left']);

                            case 'bool':
                                return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' ' . $right->getBooleanCode(), $expression['left']);

                            case 'null':
                                return new CompiledExpression('bool', $left->getCode() . ' ' . $this->_operator . ' 0', $expression['left']);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);

                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'bool':
                                    case 'double':
                                        return new CompiledExpression('bool', $variable->getName() . ' ' . $this->_operator . ' ' . $variableRight->getName(), $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $boolOperator = $left->getBooleanCode() == '1' ? $this->_zvalBoolTrueOperator : $this->_zvalBoolFalseOperator;
                                        $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);
                                        return new CompiledExpression('bool', $boolOperator . '(' . $variableRightCode . ')', $expression);

                                    default:
                                        throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot compare variable: " . $variable->getType() . " with: " . $right->getType(), $expression);
                        }
                        break;

                    case 'array':
                        switch ($right->getType()) {
                            case 'null':
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', $this->_zvalStringOperator . '(' . $variableCode . ', "")', $expression['left']);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
                                switch ($variableRight->getType()) {
                                    case 'string':
                                    case 'variable':
                                    case 'array':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRight = $compilationContext->backend->getVariableCode($variableRight);
                                        return new CompiledExpression('bool', $this->_zvalOperator . '(' . $variableCode . ', ' . $variableRight . ')', $expression);

                                    default:
                                        throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
                                }
                                break;

                            default:
                                throw new CompilerException("Unknown type: " . $right->getType(), $expression['left']);
                        }
                        break;

                    case 'string':
                        switch ($right->getType()) {
                            case 'null':
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', $this->_zvalStringOperator . '(' . $variableCode . ', "")', $expression['left']);

                            case 'string':
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', $this->_zvalStringOperator . '(' . $variableCode . ', "' . $right->getCode() . '")', $expression['left']);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
                                switch ($variableRight->getType()) {
                                    case 'string':
                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRight = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression('bool', $this->_zvalOperator . '(' . $variableCode . ', ' . $variableRight . ')', $expression);

                                    default:
                                        throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
                                }
                                break;

                            default:
                                throw new CompilerException("Unknown type: " . $right->getType(), $expression['left']);
                        }
                        break;

                    case 'variable':
                        switch ($right->getType()) {
                            case 'null':
                                $compilationContext->headersManager->add('kernel/operators');
                                $condition = $compilationContext->backend->getTypeofCondition($variable, $this->_operator, 'null', $compilationContext);
                                return new CompiledExpression('bool', $condition, $expression['left']);

                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', $this->_zvalLongOperator . '(' . $variableCode . ', ' . $right->getCode() . ')', $expression['left']);

                            case 'char':
                            case 'uchar':
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', $this->_zvalLongOperator . '(' . $variableCode . ', \'' . $right->getCode() . '\')', $expression['left']);

                            case 'bool':
                                $compilationContext->headersManager->add('kernel/operators');
                                $zvalBoolOperator = $right->getCode() == 'true' ? $this->_zvalBoolTrueOperator : $this->_zvalBoolFalseOperator;
                                return new CompiledExpression('bool', $zvalBoolOperator . '(' . $variableCode . ')', $expression['left']);

                            case 'string':
                                $compilationContext->headersManager->add('kernel/operators');
                                return new CompiledExpression('bool', $this->_zvalStringOperator . '(' . $variableCode . ', "' . $right->getCode() . '")', $expression['left']);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['left']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        return new CompiledExpression('bool', $this->_zvalLongOperator . '(' . $variableCode . ', ' . $variableRight->getName() . ')', $expression);

                                    case 'double':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        return new CompiledExpression('bool', $this->_zvalDoubleOperator . '(' . $variableCode . ', ' . $variableRight->getName() . ')', $expression);

                                    case 'bool':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        return new CompiledExpression('bool', $this->_zvalBoolOperator . '(' . $variableCode . ', ' . $variableRight->getName() . ')', $expression);

                                    case 'string':
                                    case 'variable':
                                    case 'array':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRight = $compilationContext->backend->getVariableCode($variableRight);
                                        return new CompiledExpression('bool', $this->_zvalOperator . '(' . $variableCode . ', ' . $variableRight . ')', $expression);

                                    default:
                                        throw new CompilerException("Unknown type: " . $variableRight->getType(), $expression['right']);
                                }
                                break;

                            default:
                                throw new CompilerException("Unknown type: " . $right->getType(), $expression['left']);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown type: " . $variable->getType(), $expression);
                }
                break;

            default:
                throw new CompilerException("Unknown type: " . $left->getType(), $expression);
        }
    }
}
