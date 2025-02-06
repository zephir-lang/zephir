<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Statements\Let;

use Zephir\CompilationContext as Context;
use Zephir\CompiledExpression as Expression;
use Zephir\Exception\CompilerException;
use Zephir\Exception\CompilerException as Exception;
use Zephir\Traits\VariablesTrait;
use Zephir\Variable\Variable as ZephirVariable;

use function sprintf;

/**
 * Adds/Updates an array index
 */
class ObjectProperty
{
    use VariablesTrait;

    /**
     * Compiles foo->x = {expr}.
     */
    public function assign(
        string $variable,
        ZephirVariable $symbolVariable,
        Expression $expression,
        Context $context,
        array $statement
    ): void {
        if (!$symbolVariable->isInitialized()) {
            throw new Exception(
                "Cannot mutate variable '{$variable}' because it is not initialized",
                $statement
            );
        }

        if (!$symbolVariable->isVariable()) {
            throw new Exception(
                "Variable type '{$symbolVariable->getType()}' cannot be used as object",
                $statement
            );
        }

        $propertyName = $statement['property'];
        $className    = $context->classDefinition->getCompleteName();

        if (!$symbolVariable->isInitialized()) {
            throw new Exception(
                sprintf(
                    "Cannot mutate static property '%s::%s' because it is not initialized",
                    $className,
                    $propertyName
                ),
                $statement
            );
        }

        if (!$symbolVariable->isVariable()) {
            throw new Exception(
                "Cannot use variable type: {$symbolVariable->getType()} as an object",
                $statement
            );
        }

        if ($symbolVariable->hasAnyDynamicType('unknown')) {
            throw CompilerException::cannotUseNonInitializedVariableAsObject($statement);
        }

        /**
         * Trying to use a non-object dynamic variable as object
         */
        if ($symbolVariable->hasDifferentDynamicType(['undefined', 'object'])) {
            $context->logger->warning(
                'Possible attempt to update property on non-object dynamic property',
                ['non-valid-objectupdate', $statement]
            );
        }

        /**
         * Try to check if property is implemented on related object
         */
        if ('this' == $variable) {
            if (!$context->classDefinition->hasProperty($propertyName)) {
                throw new Exception(
                    "Property '{$propertyName}' is not defined on class '{$className}'",
                    $statement
                );
            }
        }

        $codePrinter = $context->codePrinter;

        $context->headersManager->add('kernel/object');

        switch ($expression->getType()) {
            case 'null':
                $context->backend->updateProperty($symbolVariable, $propertyName, 'null', $context);
                break;

            case 'int':
            case 'long':
            case 'uint':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);
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

                        $resolvedVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
                        $context->backend->assignLong($resolvedVariable, $expression->getBooleanCode(), $context);
                        $context->backend->fetchProperty(
                            $tempVariable,
                            $symbolVariable,
                            $propertyName,
                            false,
                            $context
                        );
                        $codePrinter->output(
                            $functionName . '(' . $context->backend->getVariableCode(
                                $tempVariable
                            ) . ', ' . $context->backend->getVariableCode($resolvedVariable) . ')'
                        );
                        break;

                    case 'assign':
                        $tempVariable->initNonReferenced($context);
                        $context->backend->assignLong($tempVariable, $expression->getBooleanCode(), $context);
                        break;

                    default:
                        throw new Exception(
                            "Operator '"
                            . $statement['operator']
                            . "' is not supported for object property: "
                            . $tempVariable->getType(),
                            $statement
                        );
                }

                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'char':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);
                switch ($statement['operator']) {
                    case 'assign':
                        $tempVariable->initNonReferenced($context);
                        $context->backend->assignLong(
                            $tempVariable,
                            '\'' . $expression->getBooleanCode() . '\'',
                            $context
                        );
                        break;

                    default:
                        throw new Exception(
                            "Operator '"
                            . $statement['operator']
                            . "' is not supported for object property: "
                            . $tempVariable->getType(),
                            $statement
                        );
                }

                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'double':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);
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

                        $resolvedVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
                        $context->backend->assignDouble($resolvedVariable, $expression->getBooleanCode(), $context);
                        $context->backend->fetchProperty(
                            $tempVariable,
                            $symbolVariable,
                            $propertyName,
                            false,
                            $context
                        );
                        $codePrinter->output(
                            $functionName . '(' . $context->backend->getVariableCode(
                                $tempVariable
                            ) . ', ' . $context->backend->getVariableCode($resolvedVariable) . ')'
                        );
                        break;

                    case 'assign':
                        $tempVariable->initNonReferenced($context);
                        $context->backend->assignDouble($tempVariable, $expression->getBooleanCode(), $context);
                        break;

                    default:
                        throw new Exception(
                            "Operator '"
                            . $statement['operator']
                            . "' is not supported for object property: "
                            . $tempVariable->getType(),
                            $statement
                        );
                }

                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'string':
                $tempVariable = $context->symbolTable->getTempVariableForWrite('variable', $context, false);

                switch ($statement['operator']) {
                    case 'concat-assign':
                        $codePrinter->output(
                            'zephir_concat_self_str(&'
                            . $tempVariable->getName()
                            . ', "'
                            . $expression->getCode()
                            . '", sizeof("'
                            . $expression->getCode()
                            . '") - 1);'
                        );
                        break;
                    case 'assign':
                        /* We only can use nonReferenced variables for not refcounted stuff in ZE3 */
                        $tempVariable->initVariant($context);
                        $context->backend->assignString($tempVariable, $expression->getCode(), $context);
                        break;
                }

                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'array':
                $context->backend->updateProperty($symbolVariable, $propertyName, $expression, $context);
                break;

            case 'bool':
                $codePrinter->output('if (' . $expression->getBooleanCode() . ') {');
                $codePrinter->increaseLevel();
                $context->backend->updateProperty($symbolVariable, $propertyName, 'true', $context);
                $codePrinter->decreaseLevel();
                $codePrinter->output('} else {');
                $codePrinter->increaseLevel();
                $context->backend->updateProperty($symbolVariable, $propertyName, 'false', $context);
                $codePrinter->decreaseLevel();
                $codePrinter->output('}');
                break;

            /* unreachable code */
            case 'empty-array':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);

                $context->backend->initArray($tempVariable, $context);
                $context->backend->updateProperty($symbolVariable, $propertyName, $expression->getCode(), $context);
                $tempVariable->setIdle(true);
                break;

            case 'variable':
                $variableVariable = $context->symbolTable->getVariableForRead(
                    $expression->getCode(),
                    $context,
                    $statement
                );
                switch ($variableVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'char':
                    case 'uchar':
                        $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context, true);
                        $context->backend->assignLong($tempVariable, $variableVariable, $context);
                        $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                        $tempVariable->setIdle(true);
                        break;

                    case 'double':
                        $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context, true);
                        $context->backend->assignDouble($tempVariable, $variableVariable, $context);
                        $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                        $tempVariable->setIdle(true);
                        break;

                    case 'bool':
                        $codePrinter->output('if (' . $variableVariable->getName() . ') {');
                        $codePrinter->increaseLevel();
                        $context->backend->updateProperty($symbolVariable, $propertyName, 'true', $context);
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('} else {');
                        $codePrinter->increaseLevel();
                        $context->backend->updateProperty($symbolVariable, $propertyName, 'false', $context);
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('}');
                        break;

                    case 'array':
                    case 'string':
                    case 'variable':
                    case 'mixed':
                        $context->backend->updateProperty($symbolVariable, $propertyName, $variableVariable, $context);
                        $this->checkVariableTemporal($symbolVariable);
                        break;

                    default:
                        throw new Exception('Unknown type ' . $variableVariable->getType(), $statement);
                }
                break;

            default:
                throw new Exception("Unknown type {$expression->getType()}", $statement);
        }
    }
}
