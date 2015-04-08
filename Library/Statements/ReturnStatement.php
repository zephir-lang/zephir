<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\Expression;
use Zephir\Utils;
use Zephir\Types;

/**
 * ReturnStatement
 *
 * Return statement is used to assign variables
 */
class ReturnStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        $statement = $this->_statement;

        $codePrinter = $compilationContext->codePrinter;

        if (isset($statement['expr'])) {
            $currentMethod = $compilationContext->currentMethod;

            if ($currentMethod->isConstructor()) {
                throw new CompilerException("Constructors cannot return values", $statement['expr']);
            }

            if ($currentMethod->isVoid()) {
                throw new CompilerException("Method is marked as 'void' and it must not return any value", $statement['expr']);
            }

            /**
             * Use return member for properties on this
             */
            if ($statement['expr']['type'] == 'property-access') {
                if ($statement['expr']['left']['type'] == 'variable') {
                    if ($statement['expr']['left']['value'] == 'this') {
                        if ($statement['expr']['right']['type'] == 'variable') {
                            /**
                             * If the property is accessed on 'this', we check if the property does exist
                             */
                            $property = $statement['expr']['right']['value'];
                            $classDefinition = $compilationContext->classDefinition;
                            if (!$classDefinition->hasProperty($property)) {
                                throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a property called: '" . $property . "'", $statement['expr']['right']);
                            }

                            $compilationContext->headersManager->add('kernel/object');
                            $codePrinter->output('RETURN_MM_MEMBER(this_ptr, "' . $property . '");');
                            return;
                        }
                    }
                }
            }

            /**
             * Fetches return_value and tries to return the value directly there
             */
            $variable = $compilationContext->symbolTable->getVariable('return_value');

            $expr = new Expression($statement['expr']);
            $expr->setExpectReturn(true, $variable);
            $expr->setReadOnly(true);
            $resolvedExpr = $expr->compile($compilationContext);

            /**
             * Here we check if the variable returns a compatible type according to its type hints
             */
            if ($currentMethod->hasReturnTypes()) {
                switch ($resolvedExpr->getType()) {

                    case 'null':
                        if (!$currentMethod->areReturnTypesNullCompatible()) {
                            throw new CompilerException("Returning type: " . $resolvedExpr->getType() . " but this type is not compatible with return-type hints declared in the method", $statement['expr']);
                        }
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'char':
                    case 'uchar':
                        if (!$currentMethod->areReturnTypesIntCompatible()) {
                            throw new CompilerException("Returning type: " . $resolvedExpr->getType() . " but this type is not compatible with return-type hints declared in the method", $statement['expr']);
                        }
                        break;

                    case 'bool':
                        if (!$currentMethod->areReturnTypesBoolCompatible()) {
                            throw new CompilerException("Returning type: " . $resolvedExpr->getType() . " but this type is not compatible with return-type hints declared in the method", $statement['expr']);
                        }
                        break;

                    case 'double':
                        if (!$currentMethod->areReturnTypesDoubleCompatible()) {
                            throw new CompilerException("Returning type: " . $resolvedExpr->getType() . " but this type is not compatible with return-type hints declared in the method", $statement['expr']);
                        }
                        break;

                    case 'string':
                        if (!$currentMethod->areReturnTypesStringCompatible()) {
                            throw new CompilerException("Returning type: " . $resolvedExpr->getType() . " but this type is not compatible with return-type hints declared in the method", $statement['expr']);
                        }
                        break;

                    case 'variable':
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement['expr']);
                        switch ($symbolVariable->getType()) {

                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'char':
                            case 'uchar':
                                if (!$currentMethod->areReturnTypesIntCompatible()) {
                                    throw new CompilerException("Returning type: " . $resolvedExpr->getType() . " but this type is not compatible with return-type hints declared in the method", $statement['expr']);
                                }
                                break;

                            case 'double':
                                if (!$currentMethod->areReturnTypesDoubleCompatible()) {
                                    throw new CompilerException("Returning type: " . $resolvedExpr->getType() . " but this type is not compatible with return-type hints declared in the method", $statement['expr']);
                                }
                                break;

                            case 'string':
                                if (!$currentMethod->areReturnTypesStringCompatible()) {
                                    throw new CompilerException("Returning type: " . $resolvedExpr->getType() . " but this type is not compatible with return-type hints declared in the method", $statement['expr']);
                                }
                                break;

                            case 'bool':
                                if (!$currentMethod->areReturnTypesBoolCompatible()) {
                                    throw new CompilerException("Returning type: " . $resolvedExpr->getType() . " but this type is not compatible with return-type hints declared in the method", $statement['expr']);
                                }
                                break;

                            case 'variable':
                                break;
                        }
                        break;
                }
            }

            switch ($resolvedExpr->getType()) {

                case 'null':
                    $codePrinter->output('RETURN_MM_NULL();');
                    break;

                case 'int':
                case 'uint':
                case 'long':
                case 'char':
                case 'uchar':
                    $codePrinter->output('RETURN_MM_LONG(' . $resolvedExpr->getCode() . ');');
                    break;

                case 'bool':
                    $codePrinter->output('RETURN_MM_BOOL(' . $resolvedExpr->getBooleanCode() . ');');
                    break;

                case 'double':
                    $codePrinter->output('RETURN_MM_DOUBLE(' . $resolvedExpr->getCode() . ');');
                    break;

                case 'string':
                case 'istring':
                    $codePrinter->output('RETURN_MM_STRING("' . Utils::addSlashes($resolvedExpr->getCode()) . '", 1);');
                    break;

                case 'array':
                    if ($resolvedExpr->getCode() != 'return_value') {
                        $codePrinter->output('RETURN_CTOR(' . $resolvedExpr->getCode() . ');');
                    } else {
                        $codePrinter->output('RETURN_MM();');
                    }
                    break;

                case 'variable':

                    if (!isset($symbolVariable)) {
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement['expr']);
                    }

                    switch ($symbolVariable->getType()) {
                        case 'int':
                        case 'uint':
                        case 'long':
                        case 'char':
                        case 'uchar':
                            $codePrinter->output('RETURN_MM_LONG(' . $symbolVariable->getName() . ');');
                            break;

                        case 'double':
                            $codePrinter->output('RETURN_MM_DOUBLE(' . $symbolVariable->getName() . ');');
                            break;

                        case 'string':
                        case 'array':
                            $codePrinter->output('RETURN_CTOR(' . $resolvedExpr->getCode() . ');');
                            break;

                        case 'bool':
                            $codePrinter->output('RETURN_MM_BOOL(' . $symbolVariable->getName() . ');');
                            break;

                        case 'variable':
                            if ($symbolVariable->getName() == 'this_ptr') {
                                $codePrinter->output('RETURN_THIS();');
                            } else {
                                if ($symbolVariable->getName() != 'return_value') {
                                    if (!$symbolVariable->isExternal()) {
                                        if ($symbolVariable->isLocalOnly()) {
                                            $codePrinter->output('RETURN_LCTOR(' . $symbolVariable->getName() . ');');
                                        } else {
                                            if (!$symbolVariable->isMemoryTracked()) {
                                                $codePrinter->output('RETURN_CTOR(' . $symbolVariable->getName() . ');');
                                            } else {
                                                $codePrinter->output('RETURN_CCTOR(' . $symbolVariable->getName() . ');');
                                            }
                                        }
                                    } else {
                                        $codePrinter->output('RETVAL_ZVAL(' . $symbolVariable->getName() . ', 1, 0);');
                                        $codePrinter->output('RETURN_MM();');
                                    }
                                } else {
                                    $codePrinter->output('RETURN_MM();');
                                }
                            }
                            if ($symbolVariable->isTemporal()) {
                                $symbolVariable->setIdle(true);
                            }
                            break;

                        default:
                            throw new CompilerException("Cannot return variable '" . $symbolVariable->getType() . "'", $statement['expr']);
                    }
                    break;

                default:
                    throw new CompilerException("Cannot return '" . $resolvedExpr->getType() . "'", $statement['expr']);
            }

            return;
        }

        /**
         * Return without an expression
         */
        $codePrinter->output('RETURN_MM_NULL();');
    }
}
