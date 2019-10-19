<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements\Let;

use Zephir\ClassProperty;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Exception\IllegalOperationException;
use Zephir\Expression;

/**
 * StaticProperty.
 *
 * Updates static properties
 */
class StaticProperty
{
    /**
     * Compiles ClassName::foo = {expr}.
     *
     * @param $className
     * @param $property
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     * @throws IllegalOperationException
     *
     * @internal param string $variable
     */
    public function assignStatic($className, $property, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        $compiler = $compilationContext->compiler;
        if (!\in_array($className, ['self', 'static', 'parent'])) {
            $className = $compilationContext->getFullName($className);
            if ($compiler->isClass($className)) {
                $classDefinition = $compiler->getClassDefinition($className);
            } else {
                if ($compiler->isBundledClass($className)) {
                    $classDefinition = $compiler->getInternalClassDefinition($className);
                } else {
                    throw new CompilerException("Cannot locate class '".$className."'", $statement);
                }
            }
        } else {
            if (\in_array($className, ['self', 'static'])) {
                $classDefinition = $compilationContext->classDefinition;
            } else {
                if ('parent' == $className) {
                    $classDefinition = $compilationContext->classDefinition;
                    $extendsClass = $classDefinition->getExtendsClass();
                    if (!$extendsClass) {
                        throw new CompilerException('Cannot assign static property "'.$property.'" on parent because class '.$classDefinition->getCompleteName().' does not extend any class', $statement);
                    } else {
                        $classDefinition = $classDefinition->getExtendsClassDefinition();
                    }
                }
            }
        }

        if (!$classDefinition->hasProperty($property)) {
            throw new CompilerException("Class '".$classDefinition->getCompleteName()."' does not have a property called: '".$property."'", $statement);
        }

        /** @var ClassProperty $propertyDefinition */
        $propertyDefinition = $classDefinition->getProperty($property);
        if (!$propertyDefinition->isStatic()) {
            throw new CompilerException("Cannot access non-static property '".$classDefinition->getCompleteName().'::'.$property."'", $statement);
        }

        if ($propertyDefinition->isPrivate()) {
            if ($classDefinition != $compilationContext->classDefinition) {
                throw new CompilerException("Cannot access private static property '".$classDefinition->getCompleteName().'::'.$property."' out of its declaring context", $statement);
            }
        }

        $codePrinter = $compilationContext->codePrinter;

        $compilationContext->headersManager->add('kernel/object');
        $classEntry = $classDefinition->getClassEntry($compilationContext);

        switch ($resolvedExpr->getType()) {
            case 'null':
                $compilationContext->backend->updateStaticProperty($classEntry, $property, 'null', $compilationContext);
                break;

            case 'int':
            case 'uint':
            case 'long':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $compilationContext->backend->assignLong($tempVariable, $resolvedExpr->getBooleanCode(), $compilationContext);
                $compilationContext->backend->updateStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'char':
            case 'uchar':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $compilationContext->backend->assignLong($tempVariable, '\''.$resolvedExpr->getCode().'\'', $compilationContext);
                $compilationContext->backend->updateStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'double':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $compilationContext->backend->assignDouble($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                $compilationContext->backend->updateStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'string':
                switch ($statement['operator']) {
                    case 'assign':
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
                        break;
                    default:
                        throw new IllegalOperationException($statement, $resolvedExpr);
                }

                $compilationContext->backend->updateStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                break;

            case 'bool':
                if ('1' == $resolvedExpr->getBooleanCode()) {
                    $compilationContext->backend->updateStaticProperty($classEntry, $property, 'true', $compilationContext);
                } else {
                    if ('0' == $resolvedExpr->getBooleanCode()) {
                        $compilationContext->backend->updateStaticProperty($classEntry, $property, 'false', $compilationContext);
                    } else {
                        $codePrinter->output('if ('.$resolvedExpr->getBooleanCode().') {');
                        $codePrinter->increaseLevel();
                        $compilationContext->backend->updateStaticProperty($classEntry, $property, 'true', $compilationContext);
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('} else {');
                        $codePrinter->increaseLevel();
                        $compilationContext->backend->updateStaticProperty($classEntry, $property, 'false', $compilationContext);
                        $codePrinter->decreaseLevel();
                        $codePrinter->output('}');
                    }
                }
                break;

            case 'empty-array':
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                $compilationContext->backend->initArray($tempVariable, $compilationContext);
                $compilationContext->backend->updateStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                if ($tempVariable->isTemporal()) {
                    $tempVariable->setIdle(true);
                }
                break;

            case 'array':
                $compilationContext->backend->updateStaticProperty($classEntry, $property, $resolvedExpr, $compilationContext);
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
                        if ($compilationContext->insideCycle) {
                            $propertyCache = $compilationContext->symbolTable->getTempVariableForWrite('zend_property_info', $compilationContext);
                            $propertyCache->setMustInitNull(true);
                            $propertyCache->setReusable(false);
                            $codePrinter->output('zephir_update_static_property_ce_cache('.$classEntry.', SL("'.$property.'"), &'.$tempVariable->getName().', &'.$propertyCache->getName().');');
                        } else {
                            $compilationContext->backend->updateStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                        }
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'double':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                        $compilationContext->backend->assignDouble($tempVariable, $variableVariable, $compilationContext);
                        if ($compilationContext->insideCycle) {
                            $propertyCache = $compilationContext->symbolTable->getTempVariableForWrite('zend_property_info', $compilationContext);
                            $propertyCache->setMustInitNull(true);
                            $propertyCache->setReusable(false);
                            $codePrinter->output('zephir_update_static_property_ce_cache('.$classEntry.', SL("'.$property.'"), &'.$tempVariable->getName().', &'.$propertyCache->getName().');');
                        } else {
                            $compilationContext->backend->updateStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                        }
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'bool':
                        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
                        $compilationContext->backend->assignBool($tempVariable, $variableVariable, $compilationContext);
                        $compilationContext->backend->updateStaticProperty($classEntry, $property, $tempVariable, $compilationContext);
                        if ($tempVariable->isTemporal()) {
                            $tempVariable->setIdle(true);
                        }
                        break;

                    case 'string':
                        switch ($statement['operator']) {
                            /* @noinspection PhpMissingBreakStatementInspection */
                            case 'concat-assign':
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify('variable', $compilationContext, true);
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
                                $expression->compile($compilationContext);
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
                                $compilationContext->backend->updateStaticProperty($classEntry, $property, $variableVariable, $compilationContext);
                                if ($variableVariable->isTemporal()) {
                                    $variableVariable->setIdle(true);
                                }
                                break;
                            default:
                                throw new IllegalOperationException($statement, $variableVariable);
                        }
                        break;
                    case 'variable':
                    case 'array':
                        $compilationContext->backend->updateStaticProperty($classEntry, $property, $variableVariable, $compilationContext);
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
