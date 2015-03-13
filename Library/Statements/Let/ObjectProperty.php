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
 * ObjectProperty
 *
 * Adds/Updates an array index
 */
class ObjectProperty
{
    /**
     * Compiles foo->x = {expr}
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array $statement
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is not initialized", $statement);
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Variable type '" . $symbolVariable->getType() . "' cannot be used as object", $statement);
        }

        $propertyName = $statement['property'];
        $className = $compilationContext->classDefinition->getCompleteName();

        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate static property '" . $className . "::" . $propertyName . "' because it is not initialized", $statement);
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Cannot use variable type: " . $symbolVariable->getType() . " as an object", $statement);
        }

        if ($symbolVariable->hasAnyDynamicType('unknown')) {
            throw new CompilerException("Cannot use non-initialized variable as an object", $statement);
        }

        /**
         * Trying to use a non-object dynamic variable as object
         */
        if ($symbolVariable->hasDifferentDynamicType(array('undefined', 'object'))) {
            $compilationContext->logger->warning('Possible attempt to update property on non-object dynamic property', 'non-valid-objectupdate', $statement);
        }

        /**
         * Try to check if property is implemented on related object
         */
        if ($variable == 'this') {
            if (!$compilationContext->classDefinition->hasProperty($propertyName)) {
                throw new CompilerException("Property '" . $propertyName . "' is not defined on class '" . $className . "'", $statement);
            }
        }

        $codePrinter = $compilationContext->codePrinter;

        $compilationContext->headersManager->add('kernel/object');

        switch ($resolvedExpr->getType()) {

            case 'null':
                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
                }
                break;

            case 'int':
            case 'long':
            case 'uint':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);

                switch ($statement['operator']) {
                    case 'mul-assign':
                    case 'sub-assign':
                    case 'add-assign':

                        switch ($statement['operator']) {
                            case 'mul-assign':
                                $functionName = 'ZEPHIR_MUL_ASSIGN';
                                break;
                            case 'sub-assign':
                                $functionName = 'ZEPHIR_SUB_ASSIGN';
                                break;
                            case 'add-assign':
                                $functionName = 'ZEPHIR_ADD_ASSIGN';
                                break;
                        }

                        $resolvedVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_LONG(' . $resolvedVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
                        $codePrinter->output($functionName . '(' . $tempVariable->getName() . ', ' . $resolvedVariable->getName() . ')');
                        break;

                    case 'assign':
                        $tempVariable->initNonReferenced($compilationContext);
                        $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for object property: " . $tempVariable->getType(), $statement);
                }

                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                }
                $tempVariable->setIdle(true);
                break;

            case 'char':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);

                switch ($statement['operator']) {

                    case 'assign':
                        $tempVariable->initNonReferenced($compilationContext);
                        $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', \'' . $resolvedExpr->getBooleanCode() . '\');');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for object property: " . $tempVariable->getType(), $statement);
                }

                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                }
                $tempVariable->setIdle(true);
                break;

            case 'double':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
                switch ($statement['operator']) {
                    case 'mul-assign':
                    case 'sub-assign':
                    case 'add-assign':

                        switch ($statement['operator']) {
                            case 'mul-assign':
                                $functionName = 'ZEPHIR_MUL_ASSIGN';
                                break;
                            case 'sub-assign':
                                $functionName = 'ZEPHIR_SUB_ASSIGN';
                                break;
                            case 'add-assign':
                                $functionName = 'ZEPHIR_ADD_ASSIGN';
                                break;
                        }

                        $resolvedVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_DOUBLE(' . $resolvedVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
                        $codePrinter->output($functionName . '(' . $tempVariable->getName() . ', ' . $resolvedVariable->getName() . ')');
                        break;

                    case 'assign':
                        $tempVariable->initNonReferenced($compilationContext);
                        $codePrinter->output('ZVAL_DOUBLE(' . $tempVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for object property: " . $tempVariable->getType(), $statement);
                }

                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                }
                $tempVariable->setIdle(true);
                break;

            case 'string':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);

                switch ($statement['operator']) {
                    case 'concat-assign':
                        $codePrinter->output('zephir_concat_self_str(&' . $tempVariable->getName() . ', "' . $resolvedExpr->getCode() . '", sizeof("' . $resolvedExpr->getCode() . '")-1 TSRMLS_CC);');
                        break;
                    case 'assign':
                        $tempVariable->initNonReferenced($compilationContext);
                        $codePrinter->output('ZVAL_STRING(' . $tempVariable->getName() . ', "' . $resolvedExpr->getCode() . '", 1);');
                        break;
                }

                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                }
                $tempVariable->setIdle(true);
                break;

            case 'array':
                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $resolvedExpr->getCode() . ' TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $resolvedExpr->getCode() . ' TSRMLS_CC);');
                }
                break;

            case 'bool':
                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), (' . $resolvedExpr->getBooleanCode() . ') ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), (' . $resolvedExpr->getBooleanCode() . ') ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
                }
                break;

            case 'empty-array': /* unreachable code */
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);

                $codePrinter->output('array_init(' . $tempVariable->getName() . ');');
                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                }
                $tempVariable->setIdle(true);
                break;

            case 'variable':
                $variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($variableVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'char':
                    case 'uchar':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                        $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
                        if ($variable == 'this') {
                            $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        } else {
                            $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        }
                        $tempVariable->setIdle(true);
                        break;

                    case 'double':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                        $codePrinter->output('ZVAL_DOUBLE(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
                        if ($variable == 'this') {
                            $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        } else {
                            $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        }
                        $tempVariable->setIdle(true);
                        break;

                    case 'bool':
                        if ($variable == 'this') {
                            $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $variableVariable->getName() . ' ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
                        } else {
                            $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $variableVariable->getName() . ' ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
                        }
                        break;

                    case 'array':
                    case 'string':
                    case 'variable':
                        if ($variable == 'this') {
                            $codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $variableVariable->getName() . ' TSRMLS_CC);');
                        } else {
                            $codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $variableVariable->getName() . ' TSRMLS_CC);');
                        }
                        if ($symbolVariable->isTemporal()) {
                            $symbolVariable->setIdle(true);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown type " . $variableVariable->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
        }
    }
}
