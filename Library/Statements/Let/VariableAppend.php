<?php

/**
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
use Zephir\Exception\CompilerException;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * VariableAppend.
 *
 * Append a value to a variable
 */
class VariableAppend
{
    /**
     * Compiles foo[] = {expr}.
     *
     * @param $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param $statement
     *
     * @throws CompilerException
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is not initialized", $statement);
        }

        if ($symbolVariable->isReadOnly()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is read only", $statement);
        }

        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is local only", $statement);
        }

        /*
         * Only dynamic variables and arrays can be used as arrays
         */
        if ($symbolVariable->isNotVariableAndArray()) {
            throw new CompilerException("Cannot use variable type: '".$symbolVariable->getType()."' as an array", $statement);
        }

        if ('variable' == $symbolVariable->getType()) {
            if ($symbolVariable->hasDifferentDynamicType(['undefined', 'array'])) {
                $compilationContext->logger->warning(
                    'Possible attempt to append elements on a non-array dynamic variable',
                    ['non-array-append', $statement]
                );
            }
        }

        $compilationContext->headersManager->add('kernel/array');

        $type = $symbolVariable->getType();
        switch ($type) {
            case 'array':
            case 'variable':
            case 'mixed':
                switch ($resolvedExpr->getType()) {
                    case 'null':
                        $compilationContext->backend->addArrayEntry($symbolVariable, null, 'null', $compilationContext, $statement);
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                        $compilationContext->backend->assignLong($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                        $compilationContext->backend->addArrayEntry($symbolVariable, null, $tempVariable, $compilationContext, $statement);
                        $tempVariable->setIdle(true);
                        break;

                    case 'double':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                        $compilationContext->backend->assignDouble($tempVariable, $resolvedExpr->getCode(), $compilationContext);
                        $compilationContext->backend->addArrayEntry($symbolVariable, null, $tempVariable, $compilationContext, $statement);
                        $tempVariable->setIdle(true);
                        break;

                    case 'bool':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                        $compilationContext->backend->assignBool($tempVariable, $resolvedExpr->getBooleanCode(), $compilationContext);
                        $compilationContext->backend->addArrayEntry($symbolVariable, null, $tempVariable, $compilationContext, $statement);
                        $tempVariable->setIdle(true);
                        break;

                    case 'ulong':
                    case 'string':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                        $compilationContext->backend->assignString($tempVariable, $resolvedExpr->getBooleanCode(), $compilationContext);
                        $compilationContext->backend->addArrayEntry($symbolVariable, null, $tempVariable, $compilationContext, $statement);
                        $tempVariable->setIdle(true);
                        break;

                    case 'array':
                        $exprVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                        $compilationContext->backend->addArrayEntry($symbolVariable, null, $exprVariable, $compilationContext, $statement);
                        break;

                    case 'variable':
                    case 'mixed':
                        $exprVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                        switch ($exprVariable->getType()) {
                            case 'int':
                            case 'uint':
                            case 'long':
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                                $compilationContext->backend->assignLong($tempVariable, $exprVariable, $compilationContext);
                                $compilationContext->backend->addArrayEntry($symbolVariable, null, $tempVariable, $compilationContext, $statement);
                                $tempVariable->setIdle(true);
                                break;

                            case 'double':
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                                $compilationContext->backend->assignDouble($tempVariable, $exprVariable, $compilationContext);
                                $compilationContext->backend->addArrayEntry($symbolVariable, null, $tempVariable, $compilationContext, $statement);
                                $tempVariable->setIdle(true);
                                break;

                            case 'bool':
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                                $compilationContext->backend->assignBool($tempVariable, $exprVariable, $compilationContext);
                                $compilationContext->backend->addArrayEntry($symbolVariable, null, $tempVariable, $compilationContext, $statement);
                                $tempVariable->setIdle(true);
                                break;

                            case 'variable':
                            case 'mixed':
                            case 'string':
                            case 'array':
                                $compilationContext->backend->addArrayEntry($symbolVariable, null, $exprVariable, $compilationContext, $statement);
                                break;

                            default:
                                throw new CompilerException('Unknown type: '.$exprVariable->getType(), $statement);
                        }
                        break;

                    default:
                        throw new CompilerException('Unknown type: '.$resolvedExpr->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException('Unknown type: '.$type, $statement);
        }
    }
}
