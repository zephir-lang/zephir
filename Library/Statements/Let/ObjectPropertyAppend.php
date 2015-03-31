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
 * ObjectPropertyAppend
 *
 * Appends a value to a property
 */
class ObjectPropertyAppend
{

    /**
     * Compiles x->y[] = foo
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array $statement
     * @throws CompilerException
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, array $statement)
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is not initialized", $statement);
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Attempt to use variable type: " . $symbolVariable->getType() . " as object", $statement);
        }

        $codePrinter = $compilationContext->codePrinter;

        $property = $statement['property'];

        $compilationContext->headersManager->add('kernel/object');

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

        switch ($resolvedExpr->getType()) {

            case 'null':
                $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
                break;

            case 'bool':
                $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), (' . $resolvedExpr->getBooleanCode() . ') ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
                break;

            case 'char':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', \'' . $resolvedExpr->getCode() . '\');');
                $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'int':
            case 'long':
            case 'uint':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $resolvedExpr->getCode() . ');');
                $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'double':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $codePrinter->output('ZVAL_DOUBLE(' . $tempVariable->getName() . ', ' . $resolvedExpr->getCode() . ');');
                $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'string':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $codePrinter->output('ZVAL_STRING(' . $tempVariable->getName() . ', "' . $resolvedExpr->getCode() . '", 1);');
                $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'array':
                $variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $variableExpr->getName() . ' TSRMLS_CC);');
                break;

            case 'variable':
                $variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($variableExpr->getType()) {

                    case 'int':
                    case 'long':
                    case 'uint':
                    case 'char':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                        $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $variableExpr->getName() . ');');
                        $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'double':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                        $codePrinter->output('ZVAL_DOUBLE(' . $tempVariable->getName() . ', ' . $variableExpr->getName() . ');');
                        $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'bool':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                        $codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $variableExpr->getName() . ');');
                        $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'variable':
                    case 'string':
                    case 'array':
                    case 'resource':
                    case 'object':
                        $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $variableExpr->getName() . ' TSRMLS_CC);');
                        if ($variableExpr->isTemporal()) {
                            $variableExpr->setIdle(true);
                        }
                        break;

                    default:
                        throw new CompilerException("Variable: " . $variableExpr->getType() . " cannot be appended to array property", $statement);
                }
                break;

            default:
                throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be appended to array property", $statement);
        }
    }
}
