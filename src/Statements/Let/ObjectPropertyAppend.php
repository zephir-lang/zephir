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

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * ObjectPropertyAppend.
 *
 * Appends a value to a property
 */
class ObjectPropertyAppend
{
    /**
     * Compiles x->y[] = foo.
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     */
    public function assign(
        $variable,
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
                'Attempt to use variable type: ' . $symbolVariable->getType() . ' as object',
                $statement
            );
        }

        $codePrinter = $compilationContext->codePrinter;

        $property = $statement['property'];

        $compilationContext->headersManager->add('kernel/object');

        /*
         * Check if the variable to update is defined
         */
        if ('this' == $symbolVariable->getRealName()) {
            $classDefinition = $compilationContext->classDefinition;
            if (!$classDefinition->hasProperty($property)) {
                throw new CompilerException(
                    "Class '" . $classDefinition->getCompleteName(
                    ) . "' does not have a property called: '" . $property . "'",
                    $statement
                );
            }
        } else {
            /*
             * If we know the class related to a variable we could check if the property
             * is defined on that class
             */
            if ($symbolVariable->hasAnyDynamicType('object')) {
                $classType = current($symbolVariable->getClassTypes());
                $compiler  = $compilationContext->compiler;

                if ($compiler->isClass($classType)) {
                    $classDefinition = $compiler->getClassDefinition($classType);
                    if (!$classDefinition) {
                        throw new CompilerException(
                            'Cannot locate class definition for class: ' . $classType,
                            $statement
                        );
                    }

                    if (!$classDefinition->hasProperty($property)) {
                        throw new CompilerException(
                            "Class '" . $classType . "' does not have a property called: '" . $property . "'",
                            $statement
                        );
                    }
                }
            }
        }

        switch ($resolvedExpr->getType()) {
            case 'null':
                $compilationContext->backend->assignArrayProperty(
                    $symbolVariable,
                    $property,
                    null,
                    'null',
                    $compilationContext
                );
                break;

            case 'bool':
                $codePrinter->output('if (' . $resolvedExpr->getBooleanCode() . ') {');
                $codePrinter->increaseLevel();
                $compilationContext->backend->assignArrayProperty(
                    $symbolVariable,
                    $property,
                    null,
                    'true',
                    $compilationContext
                );
                $codePrinter->decreaseLevel();
                $codePrinter->output('} else {');
                $codePrinter->increaseLevel();
                $compilationContext->backend->assignArrayProperty(
                    $symbolVariable,
                    $property,
                    null,
                    'false',
                    $compilationContext
                );
                $codePrinter->decreaseLevel();
                $codePrinter->output('}');
                break;

            case 'char':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                    'variable',
                    $compilationContext,
                    true
                );
                $compilationContext->backend->assignLong(
                    $tempVariable,
                    '\'' . $resolvedExpr->getCode() . '\'',
                    $compilationContext
                );
                $compilationContext->backend->assignArrayProperty(
                    $symbolVariable,
                    $property,
                    null,
                    $tempVariable,
                    $compilationContext
                );
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'int':
            case 'long':
            case 'uint':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                    'variable',
                    $compilationContext,
                    true
                );
                $compilationContext->backend->assignLong($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                $compilationContext->backend->assignArrayProperty(
                    $symbolVariable,
                    $property,
                    null,
                    $tempVariable,
                    $compilationContext
                );
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'double':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                    'variable',
                    $compilationContext,
                    true
                );
                $compilationContext->backend->assignDouble(
                    $tempVariable,
                    $resolvedExpr->getCode(),
                    $compilationContext
                );
                $compilationContext->backend->assignArrayProperty(
                    $symbolVariable,
                    $property,
                    null,
                    $tempVariable,
                    $compilationContext
                );
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'string':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                    'variable',
                    $compilationContext,
                    true
                );
                $tempVariable->initVariant($compilationContext);
                $compilationContext->backend->assignString(
                    $tempVariable,
                    $resolvedExpr->getCode(),
                    $compilationContext
                );
                $compilationContext->backend->assignArrayProperty(
                    $symbolVariable,
                    $property,
                    null,
                    $tempVariable,
                    $compilationContext
                );
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'array':
                $variableExpr = $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $statement
                );
                $compilationContext->backend->assignArrayProperty(
                    $symbolVariable,
                    $property,
                    null,
                    $variableExpr,
                    $compilationContext
                );
                break;

            case 'variable':
                $variableExpr = $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $statement
                );
                switch ($variableExpr->getType()) {
                    case 'int':
                    case 'long':
                    case 'uint':
                    case 'char':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                            'variable',
                            $compilationContext,
                            true
                        );
                        $compilationContext->backend->assignLong($tempVariable, $variableExpr, $compilationContext);
                        $compilationContext->backend->assignArrayProperty(
                            $symbolVariable,
                            $property,
                            null,
                            $tempVariable,
                            $compilationContext
                        );
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'double':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                            'variable',
                            $compilationContext,
                            true
                        );
                        $compilationContext->backend->assignDouble($tempVariable, $variableExpr, $compilationContext);
                        $compilationContext->backend->assignArrayProperty(
                            $symbolVariable,
                            $property,
                            null,
                            $tempVariable,
                            $compilationContext
                        );
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'bool':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                            'variable',
                            $compilationContext,
                            true
                        );
                        $compilationContext->backend->assignBool($tempVariable, $variableExpr, $compilationContext);
                        $compilationContext->backend->assignArrayProperty(
                            $symbolVariable,
                            $property,
                            null,
                            $tempVariable,
                            $compilationContext
                        );
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'variable':
                    case 'mixed':
                    case 'string':
                    case 'array':
                    case 'resource':
                    case 'object':
                        $compilationContext->backend->assignArrayProperty(
                            $symbolVariable,
                            $property,
                            null,
                            $variableExpr,
                            $compilationContext
                        );
                        if ($variableExpr->isTemporal()) {
                            $variableExpr->setIdle(true);
                        }
                        break;

                    default:
                        throw new CompilerException(
                            'Variable: ' . $variableExpr->getType() . ' cannot be appended to array property',
                            $statement
                        );
                }
                break;

            default:
                throw new CompilerException(
                    'Expression: ' . $resolvedExpr->getType() . ' cannot be appended to array property',
                    $statement
                );
        }
    }
}
