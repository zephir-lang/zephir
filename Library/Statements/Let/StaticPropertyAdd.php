<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Exception\IllegalOperationException;
use Zephir\Expression;

/**
 * StaticPropertyAdd.
 *
 * Updates static properties
 */
class StaticPropertyAdd
{
    /**
     * Compiles ClassName::foo = {expr}.
     *
     * @param string             $className
     * @param string             $property
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     * @throws IllegalOperationException
     *
     * @internal param string $variable
     */
    public function assignStatic(
        string $className,
        string $property,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement
    ) {
        $classDefinition = $compilationContext->classLookup($className);

        if (!$propertyDefinition = $classDefinition->getProperty($property)) {
            throw new CompilerException(
                sprintf(
                    "Class '%s' does not have a property called: '%s",
                    $classDefinition->getCompleteName(),
                    $property
                ),
                $statement
            );
        }

        if (!$propertyDefinition->isStatic()) {
            throw new CompilerException(
                sprintf(
                    "Cannot access non-static property '%s::%s",
                    $classDefinition->getCompleteName(),
                    $property
                ),
                $statement
            );
        }

        if ($propertyDefinition->isPrivate()) {
            if ($classDefinition->getCompleteName() != $compilationContext->classDefinition->getCompleteName()) {
                throw new CompilerException(
                    sprintf(
                        "Cannot access private static property '%s::%s out of its declaring context",
                        $classDefinition->getCompleteName(),
                        $property
                    ),
                    $statement
                );
            }
        }

        $codePrinter = $compilationContext->codePrinter;
        $compilationContext->headersManager->add('kernel/object');

        try {
            $classEntry = $classDefinition->getClassEntry($compilationContext);
        } catch (Exception $e) {
            throw new CompilerException($e->getMessage(), $statement, $e->getCode(), $e);
        }

        switch ($resolvedExpr->getType()) {
            case 'null':
                $compilationContext->backend->updateStaticProperty($classEntry, $property, 'null', $compilationContext);
                break;

            case 'int':
            case 'uint':
            case 'long':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $compilationContext->backend->assignLong($tempVariable, $resolvedExpr->getBooleanCode(), $compilationContext);
                $compilationContext->backend->addStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'char':
            case 'uchar':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $compilationContext->backend->assignLong($tempVariable, '\''.$resolvedExpr->getCode().'\'', $compilationContext);
                $compilationContext->backend->addStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'double':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $compilationContext->backend->assignDouble($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                $compilationContext->backend->addStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'string':
                // TODO: add-assign
                if ('assign' !== $statement['operator']) {
                    throw CompilerException::illegalOperationTypeOnStaticVariable(
                        $statement['operator'],
                        $resolvedExpr->getType(),
                        $statement
                    );
                }

                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $tempVariable->initVariant($compilationContext);

                if ($resolvedExpr->getCode()) {
                    $compilationContext->backend->assignString($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                } else {
                    $codePrinter->output('ZVAL_EMPTY_STRING('.$tempVariable->getName().');');
                }

                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }

                $compilationContext->backend->addStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                break;

            case 'bool':
                if ('1' == $resolvedExpr->getBooleanCode()) {
                    $compilationContext->backend->addStaticProperty($classEntry, $property, 'true', $compilationContext);
                } else {
                    if ('0' == $resolvedExpr->getBooleanCode()) {
                        $compilationContext->backend->addStaticProperty($classEntry, $property, 'false', $compilationContext);
                    } else {
                        $codePrinter->output('if ('.$resolvedExpr->getBooleanCode().') {');
                        $codePrinter->increaseLevel();
                        $compilationContext->backend->addStaticProperty($classEntry, $property, 'true', $compilationContext);
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('} else {');
                        $codePrinter->increaseLevel();
                        $compilationContext->backend->addStaticProperty($classEntry, $property, 'false', $compilationContext);
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('}');
                    }
                }
                break;

            case 'empty-array':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $compilationContext->backend->initArray($tempVariable, $compilationContext);
                $compilationContext->backend->addStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'array':
                $compilationContext->backend->addStaticProperty($classEntry, $property, $resolvedExpr, $compilationContext);
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
                        // TODO: mul-assign, div-assign, sub-assign, add-assign
                        if ('assign' !== $statement['operator']) {
                            throw CompilerException::illegalOperationTypeOnStaticVariable(
                                $statement['operator'],
                                $variableVariable->getType(),
                                $statement
                            );
                        }

                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                            'variable',
                            $compilationContext,
                            true
                        );

                        $compilationContext->backend->assignLong($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->addStaticProperty($classEntry, $property, $tempVariable, $compilationContext);

                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'double':
                        // TODO: mul-assign, div-assign, sub-assign, add-assign
                        if ('assign' !== $statement['operator']) {
                            throw CompilerException::illegalOperationTypeOnStaticVariable(
                                $statement['operator'],
                                $variableVariable->getType(),
                                $statement
                            );
                        }

                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                            'variable',
                            $compilationContext,
                            true
                        );

                        $compilationContext->backend->assignDouble($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->addStaticProperty($classEntry, $property, $tempVariable, $compilationContext);

                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'bool':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                        $compilationContext->backend->assignBool($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->addStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'string':
                        switch ($statement['operator']) {
                            /* @noinspection PhpMissingBreakStatementInspection */
                            case 'concat-assign':
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify(
                                    'variable',
                                    $compilationContext
                                );
                                $expression = new Expression([
                                    'type' => 'static-property-access',
                                    'left' => [
                                        'value' => $statement['variable'],
                                    ],
                                    'right' => [
                                        'value' => $statement['property'],
                                    ],
                                ]);
                                $expression->setExpectReturn(true, $tempVariable);

                                try {
                                    $expression->compile($compilationContext);
                                } catch (Exception $e) {
                                    throw new CompilerException($e->getMessage(), $statement, $e->getCode(), $e);
                                }

                                $variableVariableCode = $compilationContext->backend->getVariableCode($variableVariable);
                                $tempVariableCode = $compilationContext->backend->getVariableCode($tempVariable);
                                if ('&' === substr($variableVariableCode, 0, 1)) {
                                    $compilationContext->codePrinter->output('SEPARATE_ZVAL_IF_NOT_REF('.$variableVariableCode.');');
                                } else {
                                    $compilationContext->codePrinter->output('SEPARATE_ZVAL_IF_NOT_REF(&'.$variableVariableCode.');');
                                }
                                $compilationContext->codePrinter->output('zephir_concat_function('.$variableVariableCode.', '.$tempVariableCode.', '.$variableVariableCode.');');
                                // no break
                            case 'assign':
                                $compilationContext->backend->addStaticProperty($classEntry, $property, $variableVariable, $compilationContext);
                                if ($variableVariable->isTemporal()) {
                                    $variableVariable->setIdle(true);
                                }
                                break;
                            default:
                                throw CompilerException::illegalOperationTypeOnStaticVariable(
                                    $statement['operator'],
                                    $variableVariable->getType(),
                                    $statement
                                );
                        }
                        break;
                    case 'variable':
                    case 'array':
                        $compilationContext->backend->addStaticProperty($classEntry, $property, $variableVariable, $compilationContext);
                        if ($variableVariable->isTemporal()) {
                            $variableVariable->setIdle(true);
                        }
                        break;

                    default:
                        throw new CompilerException('Unknown type '.$variableVariable->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException('Unknown type '.$resolvedExpr->getType(), $statement);
        }
    }
}
