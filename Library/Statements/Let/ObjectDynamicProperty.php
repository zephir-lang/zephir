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
use Zephir\CompiledExpression;

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
                $compilationContext->backend->updateProperty($symbolVariable, $propertyVariableName, 'null', $compilationContext);
                break;

            case 'int':
            case 'long':
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignLong($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                $compilationContext->backend->updateProperty($symbolVariable, $propertyVariableName, $tempVariable, $compilationContext);
                break;

            case 'string':
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignString($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                $compilationContext->backend->updateProperty($symbolVariable, $propertyVariableName, $tempVariable, $compilationContext);
                break;

            case 'bool':
                $value = null;
                if ($resolvedExpr->getBooleanCode() == '1') {
                    $value = 'true';
                } else if ($resolvedExpr->getBooleanCode() == '0') {
                    $value = 'false';
                } else {
                    throw new \Exception('?');
                }
                $compilationContext->backend->updateProperty($symbolVariable, $propertyVariableName, $value, $compilationContext);
                break;

            case 'empty-array':
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('array_init(' . $tempVariable->getName() . ');');
                $compilationContext->backend->updateProperty($symbolVariable, $propertyVariableName, $tempVariable, $compilationContext);
                break;

            case 'array':
                $variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                $compilationContext->backend->updateProperty($symbolVariable, $propertyVariableName, $variableVariable, $compilationContext);
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
                        $compilationContext->backend->assignLong($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->updateProperty($symbolVariable, $propertyVariableName, $tempVariable, $compilationContext);
                        break;

                    case 'bool':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $compilationContext->backend->assignBool($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->updateProperty($symbolVariable, $propertyVariableName, $tempVariable, $compilationContext);
                        break;

                    case 'string':
                    case 'variable':
                    case 'array':
                        $compilationContext->backend->updateProperty($symbolVariable, $propertyVariable, $resolvedExpr, $compilationContext);
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
