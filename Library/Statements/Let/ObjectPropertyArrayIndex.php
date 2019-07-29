<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Variable as ZephirVariable;

/**
 * ObjectPropertyArrayIndex.
 *
 * Updates object properties dynamically
 */
class ObjectPropertyArrayIndex extends ArrayIndex
{
    /**
     * Compiles x->y[z] = foo.
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext,
     * @param array              $statement
     *
     * @throws CompilerException
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is not initialized", $statement);
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException('Attempt to use variable type: '.$symbolVariable->getType().' as object', $statement);
        }

        /*
         * Update the property according to the number of array-offsets
         */
        if (1 == \count($statement['index-expr'])) {
            $this->_assignPropertyArraySingleIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
        } else {
            $this->_assignPropertyArrayMultipleIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
        }
    }

    /**
     * Compiles x->y[z] = {expr} (single offset assignment).
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     */
    protected function _assignPropertyArraySingleIndex($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, array $statement)
    {
        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /**
         * Only string/variable/int.
         */
        $indexExpression = new Expression($statement['index-expr'][0]);
        $resolvedIndex = $indexExpression->compile($compilationContext);

        switch ($resolvedIndex->getType()) {
            case 'string':
            case 'int':
            case 'uint':
            case 'ulong':
            case 'long':
            case 'variable':
                break;
            default:
                throw new CompilerException('Expression: '.$resolvedIndex->getType().' cannot be used as index without cast', $statement);
        }

        if ('variable' == $resolvedIndex->getType()) {
            $indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedIndex->getCode(), $compilationContext, $statement);
            switch ($indexVariable->getType()) {
                case 'string':
                case 'int':
                case 'uint':
                case 'ulong':
                case 'long':
                case 'variable':
                    break;
                default:
                    throw new CompilerException('Variable: '.$indexVariable->getType().' cannot be used as index without cast', $statement);
            }

            if ('variable' == $indexVariable->getType()) {
                if ($indexVariable->hasDifferentDynamicType(['undefined', 'int', 'string'])) {
                    $compilationContext->logger->warning(
                        'Possible attempt to use non string/long dynamic variable as array index',
                        ['invalid-array-offset', $statement]
                    );
                }
            }
        }

        switch ($resolvedIndex->getType()) {
            case 'int':
            case 'uint':
            case 'long':
                $indexVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                $compilationContext->backend->assignLong($indexVariable, $resolvedIndex->getCode(), $compilationContext);
                break;

            case 'string':
                $indexVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                $compilationContext->backend->assignString($indexVariable, $resolvedIndex->getCode(), $compilationContext);
                break;

            case 'variable':
                $variableIndex = $compilationContext->symbolTable->getVariableForRead($resolvedIndex->getCode(), $compilationContext, $statement['index-expr']);
                switch ($variableIndex->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                        $indexVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                        $compilationContext->backend->assignLong($indexVariable, $variableIndex, $compilationContext);
                        break;
                }
                break;
        }

        /*
         * Check if the variable to update is defined
         */
        if ('this' == $symbolVariable->getRealName()) {
            $classDefinition = $compilationContext->classDefinition;
            if (!$classDefinition->hasProperty($property)) {
                throw new CompilerException("Class '".$classDefinition->getCompleteName()."' does not have a property called: '".$property."'", $statement);
            }
        } else {
            /*
             * If we know the class related to a variable we could check if the property
             * is defined on that class
             */
            if ($symbolVariable->hasAnyDynamicType('object')) {
                $classType = current($symbolVariable->getClassTypes());
                $compiler = $compilationContext->compiler;

                if ($compiler->isClass($classType)) {
                    $classDefinition = $compiler->getClassDefinition($classType);
                    if (!$classDefinition) {
                        throw new CompilerException('Cannot locate class definition for class: '.$classType, $statement);
                    }

                    if (!$classDefinition->hasProperty($property)) {
                        throw new CompilerException("Class '".$classType."' does not have a property called: '".$property."'", $statement);
                    }
                }
            }
        }

        switch ($indexVariable->getType()) {
            case 'variable':
            case 'string':
                switch ($resolvedExpr->getType()) {
                    case 'null':
                        $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, 'null', $compilationContext);
                        break;

                    case 'bool':
                        $booleanCode = $resolvedExpr->getBooleanCode();
                        if ('1' == $booleanCode) {
                            $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, 'true', $compilationContext);
                        } elseif ('0' == $booleanCode) {
                            $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, 'false', $compilationContext);
                        } else {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                            $compilationContext->backend->assignBool($tempVariable, $booleanCode, $compilationContext);
                            $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, $tempVariable, $compilationContext);
                        }
                        break;

                    case 'int':
                    case 'long':
                    case 'uint':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $compilationContext->backend->assignLong($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                        $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, $tempVariable, $compilationContext);
                        break;

                    case 'char':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $compilationContext->backend->assignLong($tempVariable, '\''.$resolvedExpr->getCode().'\'', $compilationContext);
                        $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, $tempVariable, $compilationContext);
                        break;

                    case 'double':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $compilationContext->backend->assignDouble($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                        $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, $tempVariable, $compilationContext);
                        break;

                    case 'string':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $compilationContext->backend->assignString($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                        $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, $tempVariable, $compilationContext);
                        break;

                    case 'array':
                        $variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement['index-expr']);
                        $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, $variableExpr, $compilationContext);
                        break;

                    case 'variable':
                        $variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement['index-expr']);
                        switch ($variableExpr->getType()) {
                            case 'bool':
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                                $compilationContext->backend->assignBool($tempVariable, $variableExpr, $compilationContext);
                                $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, $tempVariable, $compilationContext);
                                break;

                            case 'int':
                            case 'long':
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                                $compilationContext->backend->assignLong($tempVariable, $variableExpr, $compilationContext);
                                $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, $tempVariable, $compilationContext);
                                break;

                            case 'variable':
                            case 'string':
                            case 'array':
                                $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, $variableExpr, $compilationContext);
                                if ($variableExpr->isTemporal()) {
                                    $variableExpr->setIdle(true);
                                }
                                break;

                            default:
                                throw new CompilerException('Cannot update variable type: '.$variableExpr->getType(), $statement);
                        }
                        break;

                    default:
                        throw new CompilerException('Variable index: '.$indexVariable->getType().' cannot be updated into array property', $statement);
                }
                break;

            default:
                throw new CompilerException('Index: '.$resolvedIndex->getType().' cannot be updated into array property', $statement);
        }
    }

    /**
     * Compiles x->y[a][b] = {expr} (multiple offset assignment).
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     */
    protected function _assignPropertyArrayMultipleIndex($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, array $statement)
    {
        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /**
         * Create a temporal zval (if needed).
         */
        $variableExpr = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

        /**
         * Only string/variable/int.
         */
        $offsetExprs = [];
        foreach ($statement['index-expr'] as $indexExpr) {
            $indexExpression = new Expression($indexExpr);

            $resolvedIndex = $indexExpression->compile($compilationContext);
            switch ($resolvedIndex->getType()) {
                case 'string':
                case 'int':
                case 'uint':
                case 'ulong':
                case 'long':
                case 'variable':
                    break;
                default:
                    throw new CompilerException('Expression: '.$resolvedIndex->getType().' cannot be used as index without cast', $statement['index-expr']);
            }

            $offsetExprs[] = $resolvedIndex;
        }

        $compilationContext->backend->assignPropertyArrayMulti($symbolVariable, $variableExpr, $property, $offsetExprs, $compilationContext);

        if ($variableExpr->isTemporal()) {
            $variableExpr->setIdle(true);
        }
    }
}
