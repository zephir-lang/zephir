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

use Exception;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * Updates object properties dynamically
 */
class ObjectDynamicProperty
{
    /**
     * Compiles foo->{x} = {expr}.
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     * @throws Exception
     */
    public function assign(
        string $variable,
        ZephirVariable $symbolVariable,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement
    ): void {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException(
                "Cannot mutate variable '" . $variable . "' because it is not initialized",
                $statement
            );
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException(
                "Variable type '" . $symbolVariable->getType() . "' cannot be used as object",
                $statement
            );
        }

        $propertyName = $statement['property'];

        $propertyVariable = $compilationContext->symbolTable->getVariableForRead(
            $propertyName,
            $compilationContext,
            $statement
        );
        if ($propertyVariable->isNotVariableAndMixedAndString()) {
            throw new CompilerException(
                "Cannot use variable type '" . $propertyVariable->getType() . "' to update object property",
                $statement
            );
        }

        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException(
                "Cannot mutate static property '" . $compilationContext->classDefinition->getCompleteName(
                ) . '::' . $propertyName . "' because it is not initialized",
                $statement
            );
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException(
                'Cannot use variable type: ' . $symbolVariable->getType() . ' as an object',
                $statement
            );
        }

        if ($symbolVariable->hasAnyDynamicType('unknown')) {
            throw new CompilerException(
                'Cannot use non-initialized variable as an object',
                $statement
            );
        }

        /**
         * Trying to use a non-object dynamic variable as object
         */
        if ($symbolVariable->hasDifferentDynamicType(['undefined', 'object', 'null'])) {
            $compilationContext->logger->warning(
                'Possible attempt to update property on non-object dynamic property',
                ['non-valid-objectupdate', $statement]
            );
        }

        $codePrinter = $compilationContext->codePrinter;
        $compilationContext->headersManager->add('kernel/object');

        $propertyVariableName = $compilationContext->symbolTable->getVariable($propertyName);

        switch ($resolvedExpr->getType()) {
            case 'null':
                $compilationContext->backend->updateProperty(
                    $symbolVariable,
                    $propertyVariableName,
                    'null',
                    $compilationContext
                );
                break;

            case 'int':
            case 'long':
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext
                );
                $compilationContext->backend->assignLong($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                $compilationContext->backend->updateProperty(
                    $symbolVariable,
                    $propertyVariableName,
                    $tempVariable,
                    $compilationContext
                );
                break;

            case 'string':
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext
                );
                $compilationContext->backend->assignString(
                    $tempVariable,
                    $resolvedExpr->getCode(),
                    $compilationContext
                );
                $compilationContext->backend->updateProperty(
                    $symbolVariable,
                    $propertyVariableName,
                    $tempVariable,
                    $compilationContext
                );
                break;

            case 'bool':
                if ('1' == $resolvedExpr->getBooleanCode()) {
                    $value = 'true';
                } elseif ('0' == $resolvedExpr->getBooleanCode()) {
                    $value = 'false';
                } else {
                    throw new Exception('?');
                }

                $compilationContext->backend->updateProperty(
                    $symbolVariable,
                    $propertyVariableName,
                    $value,
                    $compilationContext
                );
                break;

            case 'empty-array':
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext
                );
                $codePrinter->output('array_init(' . $tempVariable->getName() . ');');
                $compilationContext->backend->updateProperty(
                    $symbolVariable,
                    $propertyVariableName,
                    $tempVariable,
                    $compilationContext
                );
                break;

            case 'array':
                $variableVariable = $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $statement
                );
                $compilationContext->backend->updateProperty(
                    $symbolVariable,
                    $propertyVariableName,
                    $variableVariable,
                    $compilationContext
                );
                break;

            case 'variable':
                $variableVariable = $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $statement
                );
                switch ($variableVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'char':
                    case 'uchar':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                            'variable',
                            $compilationContext
                        );
                        $compilationContext->backend->assignLong($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->updateProperty(
                            $symbolVariable,
                            $propertyVariableName,
                            $tempVariable,
                            $compilationContext
                        );
                        break;

                    case 'bool':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                            'variable',
                            $compilationContext
                        );
                        $compilationContext->backend->assignBool($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->updateProperty(
                            $symbolVariable,
                            $propertyVariableName,
                            $tempVariable,
                            $compilationContext
                        );
                        break;

                    case 'string':
                    case 'variable':
                    case 'array':
                    case 'mixed':
                        $compilationContext->backend->updateProperty(
                            $symbolVariable,
                            $propertyVariable,
                            $resolvedExpr,
                            $compilationContext
                        );
                        if ($symbolVariable->isTemporal()) {
                            $symbolVariable->setIdle(true);
                        }
                        break;

                    default:
                        throw new CompilerException('Unknown type ' . $variableVariable->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException('Unknown type ' . $resolvedExpr->getType(), $statement);
        }
    }
}
