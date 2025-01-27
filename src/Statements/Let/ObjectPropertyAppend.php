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
use Zephir\Traits\VariablesTrait;
use Zephir\Variable\Variable as ZephirVariable;

use function current;

/**
 * Appends a value to a property
 */
class ObjectPropertyAppend
{
    use VariablesTrait;

    /**
     * Compiles x->y[] = foo.
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
        $this->checkVariableInitialized($variable, $symbolVariable, $statement);

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException(
                'Attempt to use variable type: ' . $symbolVariable->getType() . ' as object',
                $statement
            );
        }

        $codePrinter = $compilationContext->codePrinter;

        $property = $statement['property'];

        $compilationContext->headersManager->add('kernel/object');

        /**
         * Check if the variable to update is defined
         */
        if ('this' == $symbolVariable->getRealName()) {
            $classDefinition = $compilationContext->classDefinition;
            $this->checkClassHasProperty(
                $classDefinition,
                $property,
                $statement
            );
        } else {
            /**
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

                    $this->checkClassHasProperty(
                        $classDefinition,
                        $property,
                        $statement,
                        $classType
                    );
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
                $this->checkVariableTemporal($tempVariable);
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
                $this->checkVariableTemporal($tempVariable);
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
                $this->checkVariableTemporal($tempVariable);
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
                $this->checkVariableTemporal($tempVariable);
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
                        $this->checkVariableTemporal($tempVariable);
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
                        $this->checkVariableTemporal($tempVariable);
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
                        $this->checkVariableTemporal($tempVariable);
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
                        $this->checkVariableTemporal($variableExpr);
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
