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

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\Variable as ZephirVariable;
use Zephir\Detectors\ReadDetector;
use Zephir\Expression;
use Zephir\CompiledExpression;
use Zephir\Compiler;
use Zephir\Utils;
use Zephir\GlobalConstant;

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
                $codePrinter->output('ZVAL_LONG(' . $indexVariable->getName() . ', ' . $resolvedIndex->getCode() . ');');
                break;

            case 'string':
                $indexVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                $codePrinter->output('ZVAL_STRING(' . $indexVariable->getName() . ', "' . $resolvedIndex->getCode() . '", 1);');
                break;

            case 'variable':
                $variableIndex = $compilationContext->symbolTable->getVariableForRead($resolvedIndex->getCode(), $compilationContext, $statement['index-expr']);
                switch ($variableIndex->getType()) {

                    case 'int':
                    case 'uint':
                    case 'long':
                        $indexVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                        $codePrinter->output('ZVAL_LONG(' . $indexVariable->getName() . ', ' . $variableIndex->getName() . ');');
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
                        $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
                        break;

                    case 'bool':
                        $booleanCode = $resolvedExpr->getBooleanCode();
                        if ($booleanCode == '1') {
                            $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ZEPHIR_GLOBAL(global_true) TSRMLS_CC);');
                        } elseif ($booleanCode == '0') {
                            $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
                        } else {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                            $codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $booleanCode . ');');
                            $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        }
                        break;

                    case 'int':
                    case 'long':
                    case 'uint':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $resolvedExpr->getCode() . ');');
                        $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        break;

                    case 'char':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', \'' . $resolvedExpr->getCode() . '\');');
                        $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        break;

                    case 'double':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_DOUBLE(' . $tempVariable->getName() . ', ' . $resolvedExpr->getCode() . ');');
                        $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        break;

                    case 'string':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_STRING(' . $tempVariable->getName() . ', "' . $resolvedExpr->getCode() . '", 1);');
                        $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        break;

                    case 'array':
                        $variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement['index-expr']);
                        $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $variableExpr->getName() . ' TSRMLS_CC);');
                        break;

                    case 'variable':
                        $variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement['index-expr']);
                        switch ($variableExpr->getType()) {

                            case 'bool':
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                                $codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $variableExpr->getName() . ');');
                                $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                                break;

                            case 'int':
                            case 'long':
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                                $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $variableExpr->getName() . ');');
                                $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                                break;

                            case 'variable':
                            case 'string':
                            case 'array':
                                $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $variableExpr->getName() . ' TSRMLS_CC);');
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

        $keys = '';
        $numberParams = 0;
        $offsetItems = array();
        foreach ($offsetExprs as $offsetExpr) {
            switch ($offsetExpr->getType()) {

                case 'int':
                case 'uint':
                case 'long':
                case 'ulong':
                    $keys .= 'l';
                    $offsetItems[] = $offsetExpr->getCode();
                    $numberParams++;
                    break;

                case 'string':
                    $keys .= 's';
                    $offsetItems[] = 'SL("' . $offsetExpr->getCode() . '")';
                    $numberParams += 2;
                    break;

                case 'variable':
                    $variableIndex = $compilationContext->symbolTable->getVariableForRead($offsetExpr->getCode(), $compilationContext, $statement);
                    switch ($variableIndex->getType()) {

                        case 'int':
                        case 'uint':
                        case 'long':
                        case 'ulong':
                            $keys .= 'l';
                            $offsetItems[] = $variableIndex->getName();
                            $numberParams++;
                            break;

                        case 'string':
                        case 'variable':
                            $keys .= 'z';
                            $offsetItems[] = $variableIndex->getName();
                            $numberParams++;
                            break;

                        default:
                            throw new CompilerException("Variable: " . $variableIndex->getType() . " cannot be used as array index", $statement);
                    }
                    break;

                default:
                    throw new CompilerException("Value: " . $offsetExpr->getType() . " cannot be used as array index", $statement);
            }
        }

        $codePrinter->output('zephir_update_property_array_multi(' . $symbolVariable->getName() . ', SL("' . $property . '"), &' . $variableExpr->getName() . ' TSRMLS_CC, SL("' . $keys . '"), ' . $numberParams . ', ' . join(', ', $offsetItems) . ');');

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
