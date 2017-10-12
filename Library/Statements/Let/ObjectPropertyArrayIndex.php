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

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\Variable as ZephirVariable;
use Zephir\Expression;
use Zephir\CompiledExpression;

/**
 * ObjectPropertyArrayIndex
 *
 * Updates object properties dynamically
 */
class ObjectPropertyArrayIndex extends ArrayIndex
{
    /**
     * Compiles x->y[z] = {expr} (single offset assignment)
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array $statement
     * @throws CompilerException
     */
    protected function _assignPropertyArraySingleIndex($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, array $statement)
    {
        $codePrinter = $compilationContext->codePrinter;

        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /**
         * Only string/variable/int
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
                throw new CompilerException("Expression: " . $resolvedIndex->getType() . " cannot be used as index without cast", $statement);
        }

        if ($resolvedIndex->getType() == 'variable') {
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
                    throw new CompilerException("Variable: " . $indexVariable->getType() . " cannot be used as index without cast", $statement);
            }

            if ($indexVariable->getType() == 'variable') {
                if ($indexVariable->hasDifferentDynamicType(array('undefined', 'int', 'string'))) {
                    $compilationContext->logger->warning('Possible attempt to use non string/long dynamic variable as array index', 'invalid-array-offset', $statement);
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

        /**
         * Check if the variable to update is defined
         */
        if ($symbolVariable->getRealName() == 'this') {
            $classDefinition = $compilationContext->classDefinition;
            if (!$classDefinition->hasProperty($property)) {
                throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a property called: '" . $property . "'", $statement);
            }

            $propertyDefinition = $classDefinition->getProperty($property);
        } else {
            /**
             * If we know the class related to a variable we could check if the property
             * is defined on that class
             */
            if ($symbolVariable->hasAnyDynamicType('object')) {
                $classType = current($symbolVariable->getClassTypes());
                $compiler = $compilationContext->compiler;

                if ($compiler->isClass($classType)) {
                    $classDefinition = $compiler->getClassDefinition($classType);
                    if (!$classDefinition) {
                        throw new CompilerException("Cannot locate class definition for class: " . $classType, $statement);
                    }

                    if (!$classDefinition->hasProperty($property)) {
                        throw new CompilerException("Class '" . $classType . "' does not have a property called: '" . $property . "'", $statement);
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
                        if ($booleanCode == '1') {
                            $compilationContext->backend->assignArrayProperty($symbolVariable, $property, $indexVariable, 'true', $compilationContext);
                        } elseif ($booleanCode == '0') {
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
                        $compilationContext->backend->assignLong($tempVariable, '\'' . $resolvedExpr->getCode() . '\'', $compilationContext);
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
                                throw new CompilerException("Cannot update variable type: " . $variableExpr->getType(), $statement);
                        }
                        break;

                    default:
                        throw new CompilerException("Variable index: " . $indexVariable->getType() . " cannot be updated into array property", $statement);
                }
                break;

            default:
                throw new CompilerException("Index: " . $resolvedIndex->getType() . " cannot be updated into array property", $statement);
        }
    }

    /**
     * Compiles x->y[a][b] = {expr} (multiple offset assignment)
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array $statement
     * @throws CompilerException
     */
    protected function _assignPropertyArrayMultipleIndex($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, array $statement)
    {
        $codePrinter = $compilationContext->codePrinter;

        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /**
         * Create a temporal zval (if needed)
         */
        $variableExpr = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

        /**
         * Only string/variable/int
         */
        $offsetExprs = array();
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
                    throw new CompilerException("Expression: " . $resolvedIndex->getType() . " cannot be used as index without cast", $statement['index-expr']);
            }

            $offsetExprs[] = $resolvedIndex;
        }

        $compilationContext->backend->assignPropertyArrayMulti($symbolVariable, $variableExpr, $property, $offsetExprs, $compilationContext);

        if ($variableExpr->isTemporal()) {
            $variableExpr->setIdle(true);
        }
    }

    /**
     * Compiles x->y[z] = foo
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext,
     * @param array $statement
     *
     * @throws CompilerException
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is not initialized", $statement);
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Attempt to use variable type: " . $symbolVariable->getType() . " as object", $statement);
        }

        /**
         * Update the property according to the number of array-offsets
         */
        if (count($statement['index-expr']) == 1) {
            $this->_assignPropertyArraySingleIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
        } else {
            $this->_assignPropertyArrayMultipleIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
        }
    }
}
