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
 * ObjectDynamicProperty
 *
 * Updates object properties dynamically
 */
class ObjectDynamicProperty
{

    /**
     * Compiles foo->{x} = {expr}
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array $statement
     * @throws CompilerException
     * @throws \Exception
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, array $statement)
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is not initialized", $statement);
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Variable type '" . $symbolVariable->getType() . "' cannot be used as object", $statement);
        }

        $propertyName = $statement['property'];

        $propertyVariable = $compilationContext->symbolTable->getVariableForRead($propertyName, $compilationContext, $statement);
        if ($propertyVariable->isNotVariableAndString()) {
            throw new CompilerException("Cannot use variable type '" . $propertyVariable->getType() . "' to update object property", $statement);
        }

        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate static property '" . $compilationContext->classDefinition->getCompleteName() . "::" . $propertyName . "' because it is not initialized", $statement);
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
        if ($symbolVariable->hasDifferentDynamicType(array('undefined', 'object', 'null'))) {
            $compilationContext->logger->warning('Possible attempt to update property on non-object dynamic property', 'non-valid-objectupdate', $statement);
        }

        $codePrinter = $compilationContext->codePrinter;
        $compilationContext->headersManager->add('kernel/object');

        $propertyVariableName = $compilationContext->symbolTable->getVariable($propertyName);

        switch ($resolvedExpr->getType()) {

            case 'null':
                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_zval_zval(this_ptr, ' . $propertyVariableName->getName() . ', ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariableName->getName() . ', ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
                }
                break;

            case 'int':
            case 'long':
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_zval_zval(this_ptr, ' . $propertyVariableName->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariableName->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                }
                break;

            case 'string':
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('ZVAL_STRING(' . $tempVariable->getName() . ', "' . $resolvedExpr->getCode() . '", 1);');
                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_zval_zval(this_ptr, ' . $propertyVariableName->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariableName->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                }
                break;

            case 'bool':
                if ($variable == 'this') {
                    if ($resolvedExpr->getBooleanCode() == '1') {
                        $codePrinter->output('zephir_update_property_zval_zval(this_ptr, ' . $propertyVariableName->getName() . ', ZEPHIR_GLOBAL(global_true) TSRMLS_CC);');
                    } else {
                        if ($resolvedExpr->getBooleanCode() == '0') {
                            $codePrinter->output('zephir_update_property_zval_zval(this_ptr, ' . $propertyVariableName->getName() . ', ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
                        } else {
                            throw new \Exception('?');
                        }
                    }
                } else {
                    if ($resolvedExpr->getBooleanCode() == '1') {
                        $codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariableName->getName() . ', ZEPHIR_GLOBAL(global_true) TSRMLS_CC);');
                    } else {
                        if ($resolvedExpr->getBooleanCode() == '0') {
                            $codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariableName->getName() . ', ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
                        } else {
                            throw new \Exception('?');
                        }
                    }
                }
                break;

            case 'empty-array':
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('array_init(' . $tempVariable->getName() . ');');
                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_zval_zval(this_ptr, ' . $propertyVariableName->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariableName->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                }
                break;

            case 'array':
                $variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                if ($variable == 'this') {
                    $codePrinter->output('zephir_update_property_zval_zval(this_ptr, ' . $propertyVariableName->getName() . ', ' . $variableVariable->getName() . ' TSRMLS_CC);');
                } else {
                    $codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariableName->getName() . ', ' . $variableVariable->getName() . ' TSRMLS_CC);');
                }
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
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
                        $codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariableName->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        break;

                    case 'bool':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
                        if ($variable == 'this') {
                            $codePrinter->output('zephir_update_property_zval_zval(this_ptr, ' . $propertyVariableName->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        } else {
                            $codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariableName->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        }
                        break;

                    case 'string':
                    case 'variable':
                    case 'array':
                        $codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariable->getName() . ', ' . $resolvedExpr->getCode() . ' TSRMLS_CC);');
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
