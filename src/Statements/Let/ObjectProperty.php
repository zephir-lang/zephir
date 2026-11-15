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
            $classDefForCheck = $context->classDefinition;

            /**
             * If this is a closure class with an enclosing class, resolve
             * property checks against the enclosing class definition.
             * @see https://github.com/zephir-lang/zephir/issues/2497
             */
            $enclosingClassDefinition = $classDefForCheck->getEnclosingClassDefinition();
            if ($enclosingClassDefinition !== null) {
                $classDefForCheck = $enclosingClassDefinition;
            }

            if (!$classDefForCheck->hasProperty($propertyName)) {
                throw new Exception(
                    "Property '{$propertyName}' is not defined on class '{$classDefForCheck->getCompleteName()}'",
                    $statement
                );
            }
        }

        $codePrinter = $context->codePrinter;

        $context->headersManager->add('kernel/object');

        /**
         * A compound assignment is PHP's ASSIGN_OBJ_OP: one read-write fetch
         * of the property, the operator applied to it in place.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2808
         */
        if (PropertyOperator::handles($statement)) {
            (new PropertyOperator())->assign($symbolVariable, $propertyName, $expression, $context, $statement);

            return;
        }

        if ('assign' !== $statement['operator']) {
            throw new Exception(
                "Operator '" . $statement['operator'] . "' is not supported for object property",
                $statement
            );
        }

        switch ($expression->getType()) {
            case 'null':
                $context->backend->updateProperty($symbolVariable, $propertyName, 'null', $context);
                break;

            case 'int':
            case 'long':
            case 'uint':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);
                $tempVariable->initNonReferenced($context);
                $context->backend->assignLong($tempVariable, $expression->getBooleanCode(), $context);
                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'char':
            case 'uchar':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);
                $tempVariable->initNonReferenced($context);
                $context->backend->assignLong($tempVariable, $expression->getCharCode(), $context);
                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'double':
                $tempVariable = $context->symbolTable->getTempNonTrackedVariable('variable', $context);
                $tempVariable->initNonReferenced($context);
                $context->backend->assignDouble($tempVariable, $expression->getBooleanCode(), $context);
                $context->backend->updateProperty($symbolVariable, $propertyName, $tempVariable, $context);
                $tempVariable->setIdle(true);
                break;

            case 'string':
                /* We only can use nonReferenced variables for not refcounted stuff in ZE3 */
                $tempVariable = $context->symbolTable->getTempVariableForWrite('variable', $context, false);
                $tempVariable->initVariant($context);
                $context->backend->assignString($tempVariable, $expression->getCode(), $context);
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
