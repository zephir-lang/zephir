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
                $compilationContext->backend->updateProperty($symbolVariable, $propertyName, 'null', $compilationContext);
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
                        $compilationContext->backend->assignLong($resolvedVariable, $resolvedExpr->getBooleanCode(), $compilationContext);
                        $compilationContext->backend->fetchProperty($tempVariable, $symbolVariable, $propertyName, false, $compilationContext);
                        $codePrinter->output($functionName . '(' . $compilationContext->backend->getVariableCode($tempVariable) . ', ' . $compilationContext->backend->getVariableCode($resolvedVariable) . ')');
                        break;

                    case 'assign':
                        $tempVariable->initNonReferenced($compilationContext);
                        $compilationContext->backend->assignLong($tempVariable, $resolvedExpr->getBooleanCode(), $compilationContext);
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for object property: " . $tempVariable->getType(), $statement);
                }

                $compilationContext->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $compilationContext);
                $tempVariable->setIdle(true);
                break;

            case 'char':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
                switch ($statement['operator']) {
                    case 'assign':
                        $tempVariable->initNonReferenced($compilationContext);
                        $compilationContext->backend->assignLong($tempVariable, '\'' . $resolvedExpr->getBooleanCode() . '\'', $compilationContext);
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for object property: " . $tempVariable->getType(), $statement);
                }

                $compilationContext->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $compilationContext);
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
                        $compilationContext->backend->assignDouble($resolvedVariable, $resolvedExpr->getBooleanCode(), $compilationContext);
                        $compilationContext->backend->fetchProperty($tempVariable, $symbolVariable, $propertyName, false, $compilationContext);
                        $codePrinter->output($functionName . '(' . $compilationContext->backend->getVariableCode($tempVariable) . ', ' . $compilationContext->backend->getVariableCode($resolvedVariable) . ')');
                        break;

                    case 'assign':
                        $tempVariable->initNonReferenced($compilationContext);
                        $compilationContext->backend->assignDouble($tempVariable, $resolvedExpr->getBooleanCode(), $compilationContext);
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for object property: " . $tempVariable->getType(), $statement);
                }

                $compilationContext->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $compilationContext);
                $tempVariable->setIdle(true);
                break;

            case 'string':
                if ($compilationContext->backend->getName() == 'ZendEngine2') {
                    $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
                } else {
                    $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, false);
                }

                switch ($statement['operator']) {
                    case 'concat-assign':
                        $codePrinter->output('zephir_concat_self_str(&' . $tempVariable->getName() . ', "' . $resolvedExpr->getCode() . '", sizeof("' . $resolvedExpr->getCode() . '")-1 TSRMLS_CC);');
                        break;
                    case 'assign':
                        /* We only can use nonReferenced variables for not refcounted stuff in ZE3 */
                        if ($compilationContext->backend->getName() == 'ZendEngine2') {
                            $tempVariable->initNonReferenced($compilationContext);
                        } else {
                            $tempVariable->initVariant($compilationContext);
                        }
                        $compilationContext->backend->assignString($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                        break;
                }

                $compilationContext->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $compilationContext);
                $tempVariable->setIdle(true);
                break;

            case 'array':
                $compilationContext->backend->updateProperty($symbolVariable, $propertyName, $resolvedExpr, $compilationContext);
                break;

            case 'bool':
                $codePrinter->output('if (' . $resolvedExpr->getBooleanCode() . ') {');
                $codePrinter->increaseLevel();
                $compilationContext->backend->updateProperty($symbolVariable, $propertyName, 'true', $compilationContext);
                $codePrinter->decreaseLevel();
                $codePrinter->output('} else {');
                $codePrinter->increaseLevel();
                $compilationContext->backend->updateProperty($symbolVariable, $propertyName, 'false', $compilationContext);
                $codePrinter->decreaseLevel();
                $codePrinter->output('}');
                break;

            /* unreachable code */
            case 'empty-array':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);

                $compilationContext->backend->initArray($tempVariable, $compilationContext);
                $compilationContext->backend->updateProperty($symbolVariable, $propertyName, $resolvedExpr->getCode(), $compilationContext);
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
                        $compilationContext->backend->assignLong($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $compilationContext);
                        $tempVariable->setIdle(true);
                        break;

                    case 'double':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                        $compilationContext->backend->assignDouble($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $compilationContext);
                        $tempVariable->setIdle(true);
                        break;

                    case 'bool':
                        $codePrinter->output('if (' . $variableVariable->getName() . ') {');
                        $codePrinter->increaseLevel();
                        $compilationContext->backend->updateProperty($symbolVariable, $propertyName, 'true', $compilationContext);
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('} else {');
                        $codePrinter->increaseLevel();
                        $compilationContext->backend->updateProperty($symbolVariable, $propertyName, 'false', $compilationContext);
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('}');
                        break;

                    case 'array':
                    case 'string':
                    case 'variable':
                        $compilationContext->backend->updateProperty($symbolVariable, $propertyName, $variableVariable, $compilationContext);
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
