<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Exception\InvalidTypeException;
use Zephir\Expression;
use Zephir\Types;

use function Zephir\add_slashes;

/**
 * ReturnStatement.
 *
 * Return statement is used to assign variables
 */
final class ReturnStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $statement = $this->statement;

        $codePrinter = $compilationContext->codePrinter;

        if (isset($statement['expr'])) {
            $currentMethod = $compilationContext->currentMethod;

            if ($currentMethod->isConstructor()) {
                throw new CompilerException('Constructors cannot return values', $statement['expr']);
            }

            if ($currentMethod->isVoid()) {
                throw new CompilerException(
                    "Method is marked as 'void' and it must not return any value",
                    $statement['expr']
                );
            }

            /**
             * Use return member for properties on this
             */
            if ('property-access' == $statement['expr']['type']) {
                if ('variable' == $statement['expr']['left']['type']) {
                    if ('this' == $statement['expr']['left']['value']) {
                        if ('variable' == $statement['expr']['right']['type']) {
                            /**
                             * If the property is accessed on 'this', we check if the property does exist.
                             */
                            $property = $statement['expr']['right']['value'];
                            $classDefinition = $compilationContext->classDefinition;
                            if (!$classDefinition->hasProperty($property)) {
                                throw new CompilerException(
                                    sprintf(
                                        "Class '%s' does not have a property called: '%s'",
                                        $classDefinition->getCompleteName(),
                                        $property
                                    ),
                                    $statement['expr']['right']
                                );
                            }

                            $compilationContext->headersManager->add('kernel/object');
                            $codePrinter->output('RETURN_MM_MEMBER(getThis(), "'.$property.'");');

                            return;
                        }
                    }
                }
            }

            /**
             * Fetches return_value and tries to return the value directly there.
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
                    case Types::T_NULL:
                        if (!$currentMethod->areReturnTypesNullCompatible() && !$currentMethod->isMixed()) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_INT:
                    case Types::T_UINT:
                    case Types::T_LONG:
                    case Types::T_ULONG:
                    case Types::T_CHAR:
                    case Types::T_UCHAR:
                        if (!$currentMethod->areReturnTypesIntCompatible() && !$currentMethod->isMixed()) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_BOOL:
                        if (!$currentMethod->areReturnTypesBoolCompatible() && !$currentMethod->isMixed()) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_DOUBLE:
                        if (!$currentMethod->areReturnTypesDoubleCompatible() && !$currentMethod->isMixed()) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_STRING:
                    case Types::T_ISTRING:
                        if (!$currentMethod->areReturnTypesStringCompatible() && !$currentMethod->isMixed()) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_VARIABLE:
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolvedExpr->getCode(),
                            $compilationContext,
                            $statement['expr']
                        );

                        switch ($symbolVariable->getType()) {
                            case Types::T_INT:
                            case Types::T_UINT:
                            case Types::T_LONG:
                            case Types::T_ULONG:
                            case Types::T_CHAR:
                            case Types::T_UCHAR:
                                if (!$currentMethod->areReturnTypesIntCompatible() && !$currentMethod->isMixed()) {
                                    throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                                }
                                break;

                            case Types::T_DOUBLE:
                                if (!$currentMethod->areReturnTypesDoubleCompatible() && !$currentMethod->isMixed()) {
                                    throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                                }
                                break;

                            case Types::T_STRING:
                            case Types::T_ISTRING:
                                if (!$currentMethod->areReturnTypesStringCompatible() && !$currentMethod->isMixed()) {
                                    throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                                }
                                break;

                            case Types::T_BOOL:
                                if (!$currentMethod->areReturnTypesBoolCompatible() && !$currentMethod->isMixed()) {
                                    throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                                }
                                break;

                            case Types::T_VARIABLE:
                                break;
                        }
                        break;
                }
            }

            switch ($resolvedExpr->getType()) {
                case Types::T_NULL:
                    $codePrinter->output('RETURN_MM_NULL();');
                    break;

                case Types::T_INT:
                case Types::T_UINT:
                case Types::T_LONG:
                case Types::T_ULONG:
                case Types::T_CHAR:
                case Types::T_UCHAR:
                    $codePrinter->output('RETURN_MM_LONG('.$resolvedExpr->getCode().');');
                    break;

                case Types::T_BOOL:
                    $codePrinter->output('RETURN_MM_BOOL('.$resolvedExpr->getBooleanCode().');');
                    break;

                case Types::T_DOUBLE:
                    $codePrinter->output('RETURN_MM_DOUBLE('.$resolvedExpr->getCode().');');
                    break;

                case Types::T_STRING:
                case Types::T_ISTRING:
                    $compilationContext->backend->returnString(
                        add_slashes($resolvedExpr->getCode()),
                        $compilationContext
                    );
                    break;

                case Types::T_ARRAY:
                    if ('return_value' != $resolvedExpr->getCode()) {
                        $codePrinter->output('RETURN_CTOR('.$resolvedExpr->getCode().');');
                    } else {
                        $codePrinter->output('RETURN_MM();');
                    }
                    break;

                case Types::T_VARIABLE:
                    if (!isset($symbolVariable)) {
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolvedExpr->getCode(),
                            $compilationContext,
                            $statement['expr']
                        );
                    }

                    switch ($symbolVariable->getType()) {
                        case Types::T_INT:
                        case Types::T_UINT:
                        case Types::T_LONG:
                        case Types::T_ULONG:
                        case Types::T_CHAR:
                        case Types::T_UCHAR:
                            $codePrinter->output('RETURN_MM_LONG('.$symbolVariable->getName().');');
                            break;

                        case Types::T_DOUBLE:
                            $codePrinter->output('RETURN_MM_DOUBLE('.$symbolVariable->getName().');');
                            break;

                        case Types::T_STRING:
                        case Types::T_ISTRING:
                        case Types::T_ARRAY:
                            $codePrinter->output(
                                'RETURN_CTOR('.$compilationContext->backend->getVariableCode($symbolVariable).');'
                            );
                            break;

                        case Types::T_BOOL:
                            $codePrinter->output('RETURN_MM_BOOL('.$symbolVariable->getName().');');
                            break;

                        case Types::T_VARIABLE:
                            if ('this_ptr' == $symbolVariable->getName()) {
                                $codePrinter->output('RETURN_THIS();');
                            } else {
                                if ('return_value' != $symbolVariable->getName()) {
                                    if (!$symbolVariable->isExternal()) {
                                        if ($symbolVariable->isLocalOnly()) {
                                            $codePrinter->output(
                                                sprintf(
                                                    'RETURN_LCTOR(%s);',
                                                    $compilationContext->backend->getVariableCode($symbolVariable)
                                                )
                                            );
                                        } else {
                                            if (!$symbolVariable->isMemoryTracked()) {
                                                $codePrinter->output(
                                                    sprintf(
                                                        'RETURN_CTOR(%s);',
                                                        $compilationContext->backend->getVariableCode($symbolVariable)
                                                    )
                                                );
                                            } else {
                                                $codePrinter->output(
                                                    sprintf(
                                                        'RETURN_CCTOR(%s);',
                                                        $compilationContext->backend->getVariableCode($symbolVariable)
                                                    )
                                                );
                                            }
                                        }
                                    } else {
                                        $codePrinter->output(
                                            sprintf(
                                                'RETVAL_ZVAL(%s, 1, 0);',
                                                $compilationContext->backend->getVariableCode($symbolVariable)
                                            )
                                        );
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
                            throw new CompilerException(
                                sprintf("Cannot return variable '%s'", $symbolVariable->getType()),
                                $statement['expr']
                            );
                    }
                    break;

                default:
                    throw new CompilerException("Cannot return '".$resolvedExpr->getType()."'", $statement['expr']);
            }

            return;
        }

        /**
         * Return without an expression
         */
        $codePrinter->output('RETURN_MM_NULL();');
    }
}
