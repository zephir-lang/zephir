<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Arithmetical;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;
use Zephir\Variable\Variable;

/**
 * This is the base operator for commutative, associative and distributive
 * arithmetic operators
 */
class ArithmeticalBaseOperator extends AbstractOperator
{
    protected bool $literalOnly = true;

    /**
     * This tries to perform arithmetical operations.
     *
     * Probably gcc/clang will optimize them without this optimization
     *
     * @see https://en.wikipedia.org/wiki/Constant_folding
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return bool|CompiledExpression
     */
    public function optimizeConstantFolding(array $expression, CompilationContext $compilationContext)
    {
        if ('int' != $expression['left']['type'] && 'double' != $expression['left']['type']) {
            return false;
        }

        if ($compilationContext->config->get('constant-folding', 'optimizations')) {
            if ('int' == $expression['left']['type'] && 'int' == $expression['right']['type']) {
                switch ($this->operator) {
                    case '+':
                        return new CompiledExpression('int', $expression['left']['value'] + $expression['right']['value'], $expression);

                    case '-':
                        return new CompiledExpression('int', $expression['left']['value'] - $expression['right']['value'], $expression);

                    case '*':
                        return new CompiledExpression('int', $expression['left']['value'] * $expression['right']['value'], $expression);
                }
            }

            if (('double' == $expression['left']['type'] && 'double' == $expression['right']['type']) || ('double' == $expression['left']['type'] && 'int' == $expression['right']['type']) || ('int' == $expression['left']['type'] && 'double' == $expression['right']['type'])) {
                switch ($this->operator) {
                    case '+':
                        return new CompiledExpression('double', $expression['left']['value'] + $expression['right']['value'], $expression);

                    case '-':
                        return new CompiledExpression('double', $expression['left']['value'] - $expression['right']['value'], $expression);

                    case '*':
                        return new CompiledExpression('double', $expression['left']['value'] * $expression['right']['value'], $expression);
                }
            }
        }

        return false;
    }

    /**
     * Compiles the arithmetical operation.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws Exception
     * @throws CompilerException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException('Missing left part of the expression', $expression);
        }

        if (!isset($expression['right'])) {
            throw new CompilerException('Missing right part of the expression', $expression);
        }

        // Check for constant folding optimizations
        if ($optimized = $this->optimizeConstantFolding($expression, $compilationContext)) {
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
                switch ($right->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        return new CompiledExpression('int', '('.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                    case 'double':
                        return new CompiledExpression('double', '((double) '.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                    case 'bool':
                        return new CompiledExpression('int', '('.$left->getCode().' '.$this->operator.' '.$right->getBooleanCode().')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression('int', '('.$left->getCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'bool':
                                return new CompiledExpression('int', '('.$left->getCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'double':
                                return new CompiledExpression('double', '(double) ('.$left->getCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $variableRight = $compilationContext->backend->getVariableCode($variableRight);

                                return new CompiledExpression('int', '('.$left->getCode().' '.$this->operator.' zephir_get_numberval('.$variableRight.'))', $expression);

                            default:
                                throw new CompilerException("Cannot operate variable('int') with variable('".$variableRight->getType()."')", $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot operate 'int' with '".$right->getType()."'", $expression);
                }
                break;

            case 'bool':
                switch ($right->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'double':
                        return new CompiledExpression('long', '('.$left->getBooleanCode().' + '.$right->getCode().')', $expression);

                    case 'bool':
                        return new CompiledExpression('bool', '('.$left->getBooleanCode().' '.$this->bitOperator.' '.$right->getBooleanCode().')', $expression);

                    default:
                        throw new CompilerException("Cannot operate 'bool' with '".$right->getType()."'", $expression);
                }
                break;

            case 'double':
                switch ($right->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        return new CompiledExpression('double', '('.$left->getCode().' '.$this->operator.' (double) ('.$right->getCode().'))', $expression);

                    case 'double':
                        return new CompiledExpression('double', '('.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                    case 'bool':
                        return new CompiledExpression('double', '('.$left->getCode().' '.$this->operator.' '.$right->getBooleanCode().')', $expression);

                    case 'variable':
                        $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression);
                        switch ($variableRight->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'bool':
                                return new CompiledExpression('double', '('.$left->getCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'double':
                                return new CompiledExpression('double', '(double) ('.$left->getCode().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                            case 'variable':
                                $compilationContext->headersManager->add('kernel/operators');
                                $variableRight = $compilationContext->backend->getVariableCode($variableRight, $compilationContext);

                                return new CompiledExpression('double', '('.$left->getCode().' '.$this->operator.' zephir_get_numberval('.$variableRight.'))', $expression);

                            default:
                                throw new CompilerException("Cannot operate variable('double') with variable('".$variableRight->getType()."')", $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot operate 'double' with '".$right->getType()."'", $expression);
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
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                return new CompiledExpression('int', '('.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'bool':
                                        return new CompiledExpression('int', '('.$variableLeft->getName().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                                    case 'double':
                                        return new CompiledExpression('double', '((double) '.$variableLeft->getName().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRight = $compilationContext->backend->getVariableCode($variableRight, $compilationContext);

                                        return new CompiledExpression('int', '('.$variableLeft->getName().' '.$this->operator.' zephir_get_numberval('.$variableRight.'))', $expression);

                                    default:
                                        throw new CompilerException("Cannot operate variable('int') with variable('".$variableRight->getType()."')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate variable('int') with '".$right->getType()."'", $expression);
                        }
                        break;

                    case 'char':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression('int', '('.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression('int', '('.$variableLeft->getName().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRight = $compilationContext->backend->getVariableCode($variableRight, $compilationContext);

                                        return new CompiledExpression('int', '('.$variableLeft->getName().' '.$this->operator.' zephir_get_numberval('.$variableRight.'))', $expression);

                                    default:
                                        throw new CompilerException("Cannot operate variable('char') with variable('".$variableRight->getType()."')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate variable('char') with '".$right->getType()."'", $expression);
                        }
                        break;

                    case 'bool':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression('bool', '('.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '('.$left->getCode().' '.$this->bitOperator.' '.$right->getBooleanCode().')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'double':
                                        return new CompiledExpression('int', '('.$variableLeft->getName().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->bitOperator.' '.$variableRight->getName().')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRight = $compilationContext->backend->getVariableCode($variableRight, $compilationContext);

                                        return new CompiledExpression('int', '('.$variableLeft->getName().' '.$this->operator.' zephir_get_numberval('.$variableRight.'))', $expression);

                                    default:
                                        throw new CompilerException("Cannot operate variable('int') with variable('".$variableRight->getType()."')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate variable('int') with '".$right->getType()."'", $expression);
                        }
                        break;

                    case 'double':
                        switch ($right->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                return new CompiledExpression('double', '('.$left->getCode().' '.$this->operator.' (double) '.$right->getCode().')', $expression);

                            case 'double':
                                return new CompiledExpression('double', '('.$left->getCode().' '.$this->operator.' '.$right->getCode().')', $expression);

                            case 'bool':
                                return new CompiledExpression('bool', '('.$left->getCode().' '.$this->bitOperator.' '.$right->getBooleanCode().')', $expression);

                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
                                switch ($variableRight->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        return new CompiledExpression('double', '('.$variableLeft->getName().' '.$this->operator.'  (double) '.$variableRight->getName().')', $expression);

                                    case 'double':
                                        return new CompiledExpression('double', '('.$variableLeft->getName().' '.$this->operator.' '.$variableRight->getName().')', $expression);

                                    case 'bool':
                                        return new CompiledExpression('bool', '('.$variableLeft->getName().' '.$this->bitOperator.' '.$variableRight->getName().')', $expression);

                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableRight = $compilationContext->backend->getVariableCode($variableRight);

                                        return new CompiledExpression('int', '('.$variableLeft->getName().' '.$this->operator.' zephir_get_numberval('.$variableRight.'))', $expression);

                                    default:
                                        throw new CompilerException("Cannot operate variable('double') with variable('".$variableRight->getType()."')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate variable('int') with '".$right->getType()."'", $expression);
                        }
                        break;

                    case 'string':
                        throw new CompilerException("Cannot operate string variables'", $expression);
                    case 'array':
                        switch ($right->getType()) {
                            /* a(var) + a(x) */
                            case 'array':
                            case 'variable':
                                $variableRight = $compilationContext->symbolTable->getVariableForRead($right->resolve(null, $compilationContext), $compilationContext, $expression);
                                switch ($variableRight->getType()) {
                                    /* a(var) + a(var) */
                                    case 'array':
                                    case 'variable':
                                        $compilationContext->headersManager->add('kernel/operators');

                                        $expected = $this->getExpected($compilationContext, $expression);
                                        $compilationContext->backend->zvalOperator($this->zvalOperator, $expected, $variableLeft, $variableRight, $compilationContext);

                                        if ($variableLeft->isTemporal()) {
                                            $variableLeft->setIdle(true);
                                        }
                                        if ($variableRight->isTemporal()) {
                                            $variableRight->setIdle(true);
                                        }

                                        $expected->setDynamicTypes($this->getDynamicTypes($variableLeft, $variableRight));

                                        return new CompiledExpression('variable', $expected->getName(), $expression);

                                    default:
                                        throw new CompilerException("Cannot operate 'array with variable ('".$variableRight->getType()."')", $expression);
                                }
                                break;
                        }
                        break;

                    case 'variable':
                        switch ($right->getType()) {
                            /* a + 1 */
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                $compilationContext->headersManager->add('kernel/operators');
                                $op = $this->operator;

                                $op1 = $compilationContext->backend->getVariableCode($variableLeft);
                                $op2 = $right->getCode();
                                if ('double' == $right->getType()) {
                                    return new CompiledExpression('double', '(zephir_get_numberval('.$op1.') '.$op.' '.$op2.')', $expression);
                                } else {
                                    return new CompiledExpression('int', '(zephir_get_numberval('.$op1.') '.$op.' '.$op2.')', $expression);
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
                                    case 'double':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableLeft = $compilationContext->backend->getVariableCode($variableLeft);

                                        return new CompiledExpression(
                                            'double' == $variableRight->getType() ? 'double' : 'int',
                                            sprintf(
                                                '(zephir_get_numberval(%s) %s %s)',
                                                $variableLeft,
                                                $this->operator,
                                                $variableRight->getName()
                                            ),
                                            $expression
                                        );
                                        break;

                                        /* a(var) + a(bool) */
                                    case 'bool':
                                        $compilationContext->headersManager->add('kernel/operators');
                                        $variableLeft = $compilationContext->backend->getVariableCode($variableLeft);

                                        return new CompiledExpression('int', '(zephir_get_numberval('.$variableLeft.') '.$this->operator.' '.$variableRight->getName().')', $expression);
                                        break;

                                        /* a(var) + a(var) */
                                    case 'variable':
                                    case 'array':
                                        $compilationContext->headersManager->add('kernel/operators');

                                        $expected = $this->getExpected($compilationContext, $expression);
                                        $compilationContext->backend->zvalOperator($this->zvalOperator, $expected, $variableLeft, $variableRight, $compilationContext);

                                        if ($variableLeft->isTemporal()) {
                                            $variableLeft->setIdle(true);
                                        }
                                        if ($variableRight->isTemporal()) {
                                            $variableRight->setIdle(true);
                                        }

                                        $expected->setDynamicTypes($this->getDynamicTypes($variableLeft, $variableRight));

                                        return new CompiledExpression('variable', $expected->getName(), $expression);

                                    default:
                                        throw new CompilerException("Cannot operate 'variable' with variable ('".$variableRight->getType()."')", $expression);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot operate 'variable' with '".$right->getType()."'", $expression);
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

    /**
     * Returns proper dynamic types.
     *
     * @param Variable $left
     * @param Variable $right
     *
     * @return string
     */
    private function getDynamicTypes(Variable $left, Variable $right): string
    {
        if ('/' === $this->operator) {
            return 'double';
        }

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
                        return 'int';
                }
                break;
        }

        return 'double';
    }
}
