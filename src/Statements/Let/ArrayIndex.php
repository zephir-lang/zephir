<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\GlobalConstant;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * ArrayIndex.
 *
 * Adds/Updates an array index
 */
class ArrayIndex
{
    /**
     * Compiles foo[y] = {expr}.
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement): void
    {
        /*
         * Arrays must be stored in the HEAP
         */
        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is local only", $statement);
        }

        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is not initialized", $statement);
        }

        if ($symbolVariable->isReadOnly()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is read only", $statement);
        }

        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is local only", $statement);
        }

        /*
         * Only dynamic variables can be used as arrays
         */
        if ($symbolVariable->isNotVariableAndArray()) {
            throw new CompilerException("Cannot use variable type: '".$symbolVariable->getType()."' as array", $statement);
        }

        if ('variable' == $symbolVariable->getType()) {
            if ($symbolVariable->hasAnyDynamicType('unknown')) {
                throw new CompilerException('Cannot use non-initialized variable as an object', $statement);
            }

            /*
             * Trying to use a non-object dynamic variable as object
             */
            if ($symbolVariable->hasDifferentDynamicType(['undefined', 'array', 'null'])) {
                $compilationContext->logger->warning(
                    'Possible attempt to update index on a non-array dynamic variable',
                    ['non-array-update', $statement]
                );
            }
        }

        /*
         * Choose one-offset or multiple-offset functions
         */
        if (1 == \count($statement['index-expr'])) {
            $this->_assignArrayIndexSingle($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
        } else {
            $this->_assignArrayIndexMultiple($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
        }
    }

    /**
     * Resolves an item that will be assigned to an array offset.
     *
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     *
     * @return GlobalConstant|ZephirVariable
     *
     * @throws CompilerException
     */
    protected function _getResolvedArrayItem(CompiledExpression $resolvedExpr, CompilationContext $compilationContext)
    {
        switch ($resolvedExpr->getType()) {
            case 'null':
                $symbolVariable = new GlobalConstant('ZEPHIR_GLOBAL(global_null)');
                break;

            case 'int':
            case 'uint':
            case 'long':
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                $compilationContext->backend->assignLong($symbolVariable, $resolvedExpr->getCode(), $compilationContext);
                break;

            case 'char':
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                $compilationContext->backend->assignLong($symbolVariable, '\''.$resolvedExpr->getCode().'\'', $compilationContext);
                break;

            case 'double':
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                $compilationContext->backend->assignDouble($symbolVariable, $resolvedExpr->getCode(), $compilationContext);
                break;

            case 'bool':
                if ('1' == $resolvedExpr->getBooleanCode()) {
                    $symbolVariable = new GlobalConstant('ZEPHIR_GLOBAL(global_true)');
                } else {
                    if ('0' == $resolvedExpr->getBooleanCode()) {
                        $symbolVariable = new GlobalConstant('ZEPHIR_GLOBAL(global_false)');
                    } else {
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                        $compilationContext->backend->assignBool($symbolVariable, $resolvedExpr->getBooleanCode(), $compilationContext);
                    }
                }
                break;

            case 'string':
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                $compilationContext->backend->assignString($symbolVariable, $resolvedExpr->getCode(), $compilationContext);
                break;

            case 'array':
                $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $resolvedExpr->getOriginal());
                break;

            case 'variable':
                $variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $resolvedExpr->getOriginal());
                switch ($variableExpr->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                        $compilationContext->backend->assignLong($symbolVariable, $variableExpr, $compilationContext);
                        break;

                    case 'double':
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                        $compilationContext->backend->assignDouble($symbolVariable, $variableExpr, $compilationContext);
                        break;

                    case 'bool':
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                        $compilationContext->backend->assignBool($symbolVariable, $variableExpr, $compilationContext);
                        break;

                    case 'variable':
                    case 'string':
                    case 'array':
                        $symbolVariable = $variableExpr;
                        break;

                    default:
                        throw new CompilerException('Variable: '.$variableExpr->getType().' cannot be assigned to array offset', $resolvedExpr->getOriginal());
                }
                break;

            default:
                throw new CompilerException('Expression: '.$resolvedExpr->getType().' cannot be assigned to array offset', $resolvedExpr->getOriginal());
        }

        return $symbolVariable;
    }

    /**
     * Compiles foo[y] = {expr} (one offset).
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     */
    protected function _assignArrayIndexSingle($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement): void
    {
        $expression = new Expression($statement['index-expr'][0]);
        $exprIndex = $expression->compile($compilationContext);

        switch ($exprIndex->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'string':
            case 'variable':
                break;
            default:
                throw new CompilerException('Index: '.$exprIndex->getType().' cannot be used as array offset in assignment without cast', $statement['index-expr'][0]);
        }

        /**
         * Create a temporal zval (if needed).
         */
        $realSymbolVariable = $symbolVariable;
        $symbolVariable = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

        $flags = 'PH_COPY | PH_SEPARATE';

        $compilationContext->headersManager->add('kernel/array');

        switch ($exprIndex->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'string':
                $compilationContext->backend->updateArray($realSymbolVariable, $exprIndex, $symbolVariable, $compilationContext, $flags);
                break;

            case 'variable':
                $variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $compilationContext, $statement);
                switch ($variableIndex->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'string':
                    case 'variable':
                        $compilationContext->backend->updateArray($realSymbolVariable, $variableIndex, $symbolVariable, $compilationContext, $flags);
                        break;
                    default:
                        throw new CompilerException('Variable: '.$variableIndex->getType().' cannot be used as array index', $statement);
                }
                break;
            default:
                throw new CompilerException('Value: '.$exprIndex->getType().' cannot be used as array index', $statement);
        }
    }

    /**
     * Compiles foo[y][x] = {expr} (multiple offset).
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     * @throws \Zephir\Exception
     */
    protected function _assignArrayIndexMultiple(
        $variable,
        ZephirVariable $symbolVariable,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        $statement
    ): void {
        $offsetExprs = [];

        foreach ($statement['index-expr'] as $indexExpr) {
            $expression = new Expression($indexExpr);
            $expression->setReadOnly(true);
            $exprIndex = $expression->compile($compilationContext);

            switch ($exprIndex->getType()) {
                case 'int':
                case 'uint':
                case 'long':
                case 'ulong':
                case 'string':
                case 'variable':
                    break;
                default:
                    throw new CompilerException(
                        sprintf(
                            'Index: %s cannot be used as array index in assignment without cast',
                            $exprIndex->getType()
                        ),
                        $indexExpr
                    );
            }

            $offsetExprs[] = $exprIndex;
        }

        $compilationContext->headersManager->add('kernel/array');

        /**
         * Create a temporal zval (if needed).
         */
        $symbolVariable = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

        $targetVariable = $compilationContext->symbolTable->getVariableForWrite(
            $variable,
            $compilationContext,
            $statement
        );

        $compilationContext->backend->assignArrayMulti(
            $targetVariable,
            $symbolVariable,
            $offsetExprs,
            $compilationContext
        );

        if ($symbolVariable->isTemporal()) {
            $symbolVariable->setIdle(true);
        }
    }
}
