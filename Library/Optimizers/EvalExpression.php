<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers;

use Zephir\Branch;
use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\LiteralCompiledExpression;
use Zephir\Variable;

/**
 * EvalExpression.
 *
 * Resolves evaluation of expressions returning a C-int expression that can be used by 'if'/'while'/'do-while' statements
 */
class EvalExpression
{
    protected $unreachable = null;

    protected $unreachableElse = null;

    protected $usedVariables = [];

    /**
     * Skips the not operator by recursively optimizing the expression at its right.
     *
     * @param array              $expr
     * @param CompilationContext $compilationContext
     *
     * @return bool|string
     */
    public function optimizeNot($expr, CompilationContext $compilationContext)
    {
        /*
         * Compile the expression negating the evaluated expression
         */
        if ('not' == $expr['type']) {
            $conditions = $this->optimize($expr['left'], $compilationContext);
            if (false !== $conditions) {
                if (null !== $this->unreachable) {
                    $this->unreachable = !$this->unreachable;
                }
                if (null !== $this->unreachableElse) {
                    $this->unreachableElse = !$this->unreachableElse;
                }

                return '!('.$conditions.')';
            }
        }

        return false;
    }

    /**
     * Optimizes expressions.
     *
     * @param $exprRaw
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     *
     * @return bool|string
     */
    public function optimize($exprRaw, CompilationContext $compilationContext)
    {
        $conditions = $this->optimizeNot($exprRaw, $compilationContext);

        if (false !== $conditions) {
            return $conditions;
        }

        /*
         * Discard first level parentheses
         */
        if ('list' == $exprRaw['type']) {
            $expr = new Expression($exprRaw['left']);
        } else {
            $expr = new Expression($exprRaw);
        }

        $expr->setReadOnly(true);
        $expr->setEvalMode(true);
        $compiledExpression = $expr->compile($compilationContext);

        /*
         * Possible corrupted expression?
         */
        if (!\is_object($compiledExpression)) {
            throw new CompilerException('Corrupted expression: '.$exprRaw['type'], $exprRaw);
        }

        /*
         * Generate the condition according to the value returned by the evaluated expression
         */
        switch ($compiledExpression->getType()) {
            case 'null':
                $this->unreachable = true;

                return '0';

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
                $code = $compiledExpression->getCode();
                if (is_numeric($code)) {
                    if ('1' == $code) {
                        $this->unreachableElse = true;
                    } else {
                        $this->unreachable = true;
                    }
                }

                return $code;

            case 'char':
            case 'uchar':
                return $compiledExpression->getCode();

            case 'bool':
                $code = $compiledExpression->getBooleanCode();
                if ('1' == $code) {
                    $this->unreachableElse = true;
                } else {
                    if ('0' == $code) {
                        $this->unreachable = true;
                    }
                }

                return $code;

            case 'variable':
                $variableRight = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $compilationContext, $exprRaw);
                $possibleValue = $variableRight->getPossibleValue();
                if (\is_object($possibleValue)) {
                    $possibleValueBranch = $variableRight->getPossibleValueBranch();
                    if ($possibleValueBranch instanceof Branch) {
                        /*
                         * Check if the possible value was assigned in the root branch
                         */
                        if (Branch::TYPE_ROOT == $possibleValueBranch->getType()) {
                            if ($possibleValue instanceof LiteralCompiledExpression) {
                                switch ($possibleValue->getType()) {
                                    case 'null':
                                        $this->unreachable = true;
                                        break;

                                    case 'bool':
                                        if ('0' == $possibleValue->getBooleanCode()) {
                                            $this->unreachable = true;
                                        } else {
                                            $this->unreachableElse = true;
                                        }
                                        break;

                                    case 'int':
                                        if (!(int) ($possibleValue->getCode())) {
                                            $this->unreachable = true;
                                        } else {
                                            $this->unreachableElse = true;
                                        }
                                        break;

                                    case 'double':
                                        if (!(float) ($possibleValue->getCode())) {
                                            $this->unreachable = true;
                                        } else {
                                            $this->unreachableElse = true;
                                        }
                                        break;

                                    default:
                                        //echo $possibleValue->getType();
                                }
                            }
                        }
                    }
                }

                $this->usedVariables[] = $variableRight->getName();

                /**
                 * Evaluate the variable
                 */
                switch ($variableRight->getType()) {
                    case 'int':
                    case 'uint':
                    case 'char':
                    case 'uchar':
                    case 'long':
                    case 'ulong':
                        return $variableRight->getName();

                    case 'string':
                        return '!('.$compilationContext->backend->ifVariableValueUndefined($variableRight, $compilationContext, true, false).')';

                    case 'bool':
                        return $variableRight->getName();

                    case 'double':
                        return $variableRight->getName();

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $variableRightCode = $compilationContext->backend->getVariableCode($variableRight);

                        return 'zephir_is_true('.$variableRightCode.')';

                    default:
                        throw new CompilerException("Variable can't be evaluated ".$variableRight->getType(), $exprRaw);
                }
                break;

            default:
                throw new CompilerException('Expression '.$compiledExpression->getType()." can't be evaluated", $exprRaw);
        }
    }

    /**
     * Checks if the evaluation produce unreachable code.
     *
     * @return bool
     */
    public function isUnreachable()
    {
        return $this->unreachable;
    }

    /**
     * Checks if the evaluation not produce unreachable code.
     *
     * @return bool
     */
    public function isUnreachableElse()
    {
        return $this->unreachableElse;
    }

    /**
     * Returns the variable evaluated by the EvalExpression.
     *
     * @return Variable
     */
    public function getEvalVariable()
    {
        return end($this->usedVariables);
    }

    /**
     * @return array
     */
    public function getUsedVariables()
    {
        return $this->usedVariables;
    }
}
