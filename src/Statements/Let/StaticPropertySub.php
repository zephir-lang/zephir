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
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Exception\IllegalOperationException;
use Zephir\Traits\VariablesTrait;

use function sprintf;

/**
 * StaticPropertySub.
 *
 * Updates static properties
 */
class StaticPropertySub
{
    use VariablesTrait;

    protected string $methodName = 'subStaticProperty';

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
     */
    public function assignStatic(
        string $className,
        string $property,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement
    ): void {
        $classDefinition = $compilationContext->classLookup($className);
        $method          = $this->methodName;

        if (!$propertyDefinition = $classDefinition->getProperty($property)) {
            throw CompilerException::classDoesNotHaveProperty(
                $classDefinition->getCompleteName(),
                $property,
                $statement
            );
        }

        $this->checkAccessNonStaticProperty(
            $propertyDefinition,
            $classDefinition,
            $property,
            $statement
        );

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
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                    'variable',
                    $compilationContext,
                    true
                );
                $compilationContext->backend->assignLong(
                    $tempVariable,
                    $resolvedExpr->getBooleanCode(),
                    $compilationContext
                );
                $compilationContext->backend->$method(
                    $classEntry,
                    $property,
                    $tempVariable,
                    $compilationContext
                );
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'char':
            case 'uchar':
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
                $compilationContext->backend->$method(
                    $classEntry,
                    $property,
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
                $compilationContext->backend->$method(
                    $classEntry,
                    $property,
                    $tempVariable,
                    $compilationContext
                );
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'string':
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext,
                    true
                );
                $tempVariable->initVariant($compilationContext);

                if ($resolvedExpr->getCode()) {
                    $compilationContext->backend->assignString(
                        $tempVariable,
                        $resolvedExpr->getCode(),
                        $compilationContext
                    );
                } else {
                    $codePrinter->output('ZVAL_EMPTY_STRING(' . $tempVariable->getName() . ');');
                }

                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }

                $compilationContext->backend->$method(
                    $classEntry,
                    $property,
                    $tempVariable,
                    $compilationContext
                );
                break;

            case 'bool':
                if ('1' == $resolvedExpr->getBooleanCode()) {
                    $compilationContext->backend->$method(
                        $classEntry,
                        $property,
                        'true',
                        $compilationContext
                    );
                } else {
                    if ('0' == $resolvedExpr->getBooleanCode()) {
                        $compilationContext->backend->$method(
                            $classEntry,
                            $property,
                            'false',
                            $compilationContext
                        );
                    } else {
                        $codePrinter->output('if (' . $resolvedExpr->getBooleanCode() . ') {');
                        $codePrinter->increaseLevel();
                        $compilationContext->backend->$method(
                            $classEntry,
                            $property,
                            'true',
                            $compilationContext
                        );
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('} else {');
                        $codePrinter->increaseLevel();
                        $compilationContext->backend->$method(
                            $classEntry,
                            $property,
                            'false',
                            $compilationContext
                        );
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('}');
                    }
                }
                break;

            case 'empty-array':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                    'variable',
                    $compilationContext,
                    true
                );
                $compilationContext->backend->initArray($tempVariable, $compilationContext);
                $compilationContext->backend->$method(
                    $classEntry,
                    $property,
                    $tempVariable,
                    $compilationContext
                );
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'array':
                $compilationContext->backend->$method(
                    $classEntry,
                    $property,
                    $resolvedExpr,
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
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                            'variable',
                            $compilationContext,
                            true
                        );

                        $compilationContext->backend->assignLong($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->$method(
                            $classEntry,
                            $property,
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
                            $variableVariable,
                            $compilationContext
                        );
                        $compilationContext->backend->$method(
                            $classEntry,
                            $property,
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
                        $compilationContext->backend->assignBool($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->$method(
                            $classEntry,
                            $property,
                            $tempVariable,
                            $compilationContext
                        );
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;
                    default:
                        $compilationContext->backend->$method(
                            $classEntry,
                            $property,
                            $variableVariable,
                            $compilationContext
                        );
                        if ($variableVariable->isTemporal()) {
                            $variableVariable->setIdle(true);
                        }
                        break;
                }

                break;

            default:
                throw new CompilerException('Unknown type ' . $resolvedExpr->getType(), $statement);
        }
    }
}
