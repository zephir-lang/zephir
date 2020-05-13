<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\GlobalConstant;
use Zephir\Variable;

/**
 * Zephir\Expression\NativeArray.
 *
 * Resolves expressions that create arrays
 */
class NativeArray
{
    protected $expecting = true;

    protected $readOnly = false;

    protected $noisy = true;

    protected $expectingVariable;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     *
     * @param bool     $expecting
     * @param Variable $expectingVariable
     */
    public function setExpectReturn($expecting, Variable $expectingVariable = null)
    {
        $this->expecting = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     *
     * @param bool $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->readOnly = $readOnly;
    }

    /**
     * Sets whether the expression must be resolved in "noisy" mode.
     *
     * @param bool $noisy
     */
    public function setNoisy($noisy)
    {
        $this->noisy = $noisy;
    }

    /**
     * Resolves an item to be added in an array.
     *
     * @param CompiledExpression $exprCompiled
     * @param CompilationContext $compilationContext
     *
     * @return Variable
     */
    public function getArrayValue(CompiledExpression $exprCompiled, CompilationContext $compilationContext)
    {
        switch ($exprCompiled->getType()) {
            case 'int':
            case 'uint':
            case 'long':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignLong($tempVar, $exprCompiled->getCode(), $compilationContext);

                return $tempVar;

            case 'char':
            case 'uchar':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignLong($tempVar, '\''.$exprCompiled->getCode().'\'', $compilationContext);

                return $tempVar;

            case 'double':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignDouble($tempVar, $exprCompiled->getCode(), $compilationContext);

                return $tempVar;

            case 'bool':
                if ('true' == $exprCompiled->getCode()) {
                    return new GlobalConstant('ZEPHIR_GLOBAL(global_true)');
                }

                if ('false' == $exprCompiled->getCode()) {
                    return new GlobalConstant('ZEPHIR_GLOBAL(global_false)');
                }

                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignBool($tempVar, $exprCompiled->getCode(), $compilationContext);

                return $tempVar;

            case 'null':
                return new GlobalConstant('ZEPHIR_GLOBAL(global_null)');

            case 'string':
            case 'ulong':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignString($tempVar, $exprCompiled->getCode(), $compilationContext);

                return $tempVar;

            case 'array':
                return $compilationContext->symbolTable->getVariableForRead($exprCompiled->getCode(), $compilationContext, $exprCompiled->getOriginal());

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiled->getCode(), $compilationContext, $exprCompiled->getOriginal());
                switch ($itemVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $compilationContext->backend->assignLong($tempVar, $itemVariable, $compilationContext);

                        return $tempVar;

                    case 'double':
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $compilationContext->backend->assignDouble($tempVar, $itemVariable, $compilationContext);

                        return $tempVar;

                    case 'bool':
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $compilationContext->backend->assignBool($tempVar, $itemVariable, $compilationContext);

                        return $tempVar;

                    case 'string':
                    case 'variable':
                    case 'array':
                        return $itemVariable;

                    default:
                        throw new CompilerException('Unknown '.$itemVariable->getType(), $itemVariable);
                }
                break;

            default:
                throw new CompilerException('Unknown', $exprCompiled);
        }
    }

    /**
     * Compiles an array initialization.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        /*
         * Resolves the symbol that expects the value
         */
        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
                if ('variable' != $symbolVariable->getType() && 'array' != $symbolVariable->getType()) {
                    throw new CompilerException('Cannot use variable type: '.$symbolVariable->getType().' as an array', $expression);
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('array', $compilationContext, $expression);
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('array', $compilationContext, $expression);
        }

        if (!isset($expression['left'])) {
            if ($this->expectingVariable) {
                $symbolVariable->initVariant($compilationContext);
            }
            /*+
             * Mark the variable as an array
             */
            $symbolVariable->setDynamicTypes('array');

            return new CompiledExpression('array', $symbolVariable->getRealName(), $expression);
        }

        $codePrinter = $compilationContext->codePrinter;

        $compilationContext->headersManager->add('kernel/array');

        /**
         * This calculates a prime number bigger than the current array size to possibly
         * reduce hash collisions when adding new members to the array.
         */
        $arrayLength = \count($expression['left']);
        if ($arrayLength >= 33 && \function_exists('gmp_nextprime')) {
            $arrayLength = gmp_strval(gmp_nextprime($arrayLength - 1));
        }
        $oldSymbolVariable = $symbolVariable;
        if ($this->expectingVariable && $symbolVariable->getVariantInits() >= 1) {
            $symbolVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
            $symbolVariable->initVariant($compilationContext);
            $compilationContext->backend->initArray($symbolVariable, $compilationContext, $arrayLength > 0 ? $arrayLength : null);
            $symbolVariable->setDynamicTypes('array');
        } else {
            if ($this->expectingVariable) {
                $symbolVariable->initVariant($compilationContext);
            }
            /*+
             * Mark the variable as an array
             */
            $symbolVariable->setDynamicTypes('array');
            $compilationContext->backend->initArray($symbolVariable, $compilationContext, $arrayLength > 0 ? $arrayLength : null);
        }
        foreach ($expression['left'] as $item) {
            if (isset($item['key'])) {
                $key = null;
                $exprKey = new Expression($item['key']);
                $resolvedExprKey = $exprKey->compile($compilationContext);

                switch ($resolvedExprKey->getType()) {
                    case 'string':
                        $expr = new Expression($item['value']);
                        $resolvedExpr = $expr->compile($compilationContext);
                        switch ($resolvedExpr->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                                $compilationContext->backend->addArrayEntry($symbolVariable, $resolvedExprKey, $resolvedExpr, $compilationContext);
                                break;

                            case 'bool':
                                $compilationContext->headersManager->add('kernel/array');
                                if ('true' == $resolvedExpr->getCode()) {
                                    $compilationContext->backend->updateArray($symbolVariable, $resolvedExprKey, 'true', $compilationContext, 'PH_COPY | PH_SEPARATE');
                                } else {
                                    $compilationContext->backend->updateArray($symbolVariable, $resolvedExprKey, 'false', $compilationContext, 'PH_COPY | PH_SEPARATE');
                                }
                                break;

                            case 'string':
                                $compilationContext->backend->addArrayEntry($symbolVariable, $resolvedExprKey, $resolvedExpr, $compilationContext);
                                break;

                            case 'null':
                                $compilationContext->backend->updateArray($symbolVariable, $resolvedExprKey, 'null', $compilationContext, 'PH_COPY | PH_SEPARATE');
                                break;

                            case 'array':
                            case 'variable':
                                $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                                $compilationContext->backend->updateArray($symbolVariable, $resolvedExprKey, $valueVariable, $compilationContext, 'PH_COPY | PH_SEPARATE');
                                if ($valueVariable->isTemporal()) {
                                    $valueVariable->setIdle(true);
                                }
                                break;

                            default:
                                throw new CompilerException('Invalid value type: '.$resolvedExpr->getType(), $item['value']);
                        }
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $expr = new Expression($item['value']);
                        $resolvedExpr = $expr->compile($compilationContext);
                        switch ($resolvedExpr->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                            case 'double':
                            case 'string':
                                $compilationContext->backend->addArrayEntry($symbolVariable, $resolvedExprKey, $resolvedExpr, $compilationContext);
                                break;

                            case 'bool':
                                if ('true' == $resolvedExpr->getCode()) {
                                    $compilationContext->backend->updateArray($symbolVariable, $resolvedExprKey, 'true', $compilationContext, 'PH_COPY');
                                } else {
                                    $compilationContext->backend->updateArray($symbolVariable, $resolvedExprKey, 'false', $compilationContext, 'PH_COPY');
                                }
                                break;

                            case 'null':
                                $compilationContext->backend->updateArray($symbolVariable, $resolvedExprKey, 'null', $compilationContext, 'PH_COPY');
                                break;

                            case 'array':
                            case 'variable':
                                $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                                $compilationContext->backend->updateArray($symbolVariable, $resolvedExprKey, $valueVariable, $compilationContext, 'PH_COPY');
                                if ($valueVariable->isTemporal()) {
                                    $valueVariable->setIdle(true);
                                }
                                break;

                            default:
                                throw new CompilerException('Invalid value type: '.$item['value']['type'], $item['value']);
                        }
                        break;

                    case 'variable':
                        $variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExprKey->getCode(), $compilationContext, $item['key']);
                        switch ($variableVariable->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'ulong':
                                $expr = new Expression($item['value']);
                                $resolvedExpr = $expr->compile($compilationContext);
                                switch ($resolvedExpr->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                    case 'bool':
                                    case 'double':
                                    case 'null':
                                    case 'string':
                                        $compilationContext->backend->addArrayEntry($symbolVariable, $resolvedExprKey, $resolvedExpr, $compilationContext);
                                        break;

                                    case 'variable':
                                        $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                                        $compilationContext->backend->updateArray($symbolVariable, $resolvedExprKey, $valueVariable, $compilationContext, 'PH_COPY');
                                        if ($valueVariable->isTemporal()) {
                                            $valueVariable->setIdle(true);
                                        }
                                        break;

                                    default:
                                        throw new CompilerException('Invalid value type: '.$item['value']['type'], $item['value']);
                                }
                                break;

                            case 'string':
                                $expr = new Expression($item['value']);
                                $resolvedExpr = $expr->compile($compilationContext);
                                switch ($resolvedExpr->getType()) {
                                    case 'int':
                                    case 'uint':
                                    case 'long':
                                    case 'ulong':
                                        $codePrinter->output('add_assoc_long_ex('.$symbolVariable->getName().', Z_STRVAL_P('.$resolvedExprKey->getCode().'), Z_STRLEN_P('.$item['key']['value'].'), '.$resolvedExpr->getCode().');');
                                        break;

                                    case 'double':
                                        $codePrinter->output('add_assoc_double_ex('.$symbolVariable->getName().', Z_STRVAL_P('.$resolvedExprKey->getCode().'), Z_STRLEN_P('.$item['key']['value'].'), '.$resolvedExpr->getCode().');');
                                        break;

                                    case 'bool':
                                        $codePrinter->output('add_assoc_bool_ex('.$symbolVariable->getName().', Z_STRVAL_P('.$resolvedExprKey->getCode().'), Z_STRLEN_P('.$item['key']['value'].'), '.$resolvedExpr->getBooleanCode().');');
                                        break;

                                    case 'string':
                                        $compilationContext->backend->addArrayEntry($symbolVariable, $resolvedExprKey, $resolvedExpr, $compilationContext);
                                        break;

                                    case 'null':
                                        $codePrinter->output('add_assoc_null_ex('.$symbolVariable->getName().', Z_STRVAL_P('.$resolvedExprKey->getCode().'), Z_STRLEN_P('.$item['key']['value'].') + 1);');
                                        break;

                                    case 'variable':
                                        $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                                        $compilationContext->backend->updateArray($symbolVariable, $resolvedExprKey, $resolvedExpr, $compilationContext);
                                        if ($valueVariable->isTemporal()) {
                                            $valueVariable->setIdle(true);
                                        }
                                        break;

                                    default:
                                        throw new CompilerException('Invalid value type: '.$resolvedExpr->getType(), $item['value']);
                                }
                                break;

                            case 'variable':
                                $expr = new Expression($item['value']);
                                $resolvedExpr = $expr->compile($compilationContext);
                                switch ($resolvedExpr->getType()) {
                                    case 'null':
                                    case 'bool':
                                        $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                                        $compilationContext->backend->updateArray($symbolVariable, $variableVariable, $valueVariable, $compilationContext);

                                        if ($valueVariable->isTemporal()) {
                                            $valueVariable->setIdle(true);
                                        }
                                        break;

                                    case 'variable':
                                        $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                                        $compilationContext->backend->updateArray($symbolVariable, $variableVariable, $valueVariable, $compilationContext);

                                        if ($valueVariable->isTemporal()) {
                                            $valueVariable->setIdle(true);
                                        }
                                        break;

                                    default:
                                        throw new CompilerException('Invalid value type: '.$item['value']['type'], $item['value']);
                                }
                                break;

                            default:
                                throw new CompilerException('Cannot use variable type: '.$variableVariable->getType().' as array index', $item['key']);
                        }
                        break;

                    default:
                        throw new CompilerException('Invalid key type: '.$resolvedExprKey->getType(), $item['key']);
                }
            } else {
                $expr = new Expression($item['value']);
                $resolvedExpr = $expr->compile($compilationContext);
                $itemVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                $symbol = $compilationContext->backend->getVariableCode($symbolVariable);
                $item = $compilationContext->backend->resolveValue($itemVariable, $compilationContext);
                $codePrinter->output('zephir_array_fast_append('.$symbol.', '.$item.');');
                if ($itemVariable->isTemporal()) {
                    $itemVariable->setIdle(true);
                }
            }
        }

        return new CompiledExpression('array', $symbolVariable->getRealName(), $expression);
    }
}
