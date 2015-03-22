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

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\Variable as ZephirVariable;
use Zephir\Detectors\ReadDetector;
use Zephir\Expression;
use Zephir\CompiledExpression;
use Zephir\Compiler;
use Zephir\Utils;
use Zephir\GlobalConstant;

/**
 * VariableAppend
 *
 * Append a value to a variable
 */
class VariableAppend
{
    /**
     * Compiles foo[] = {expr}
     *
     * @param $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param $statement
     * @throws CompilerException
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is not initialized", $statement);
        }

        if ($symbolVariable->isReadOnly()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is read only", $statement);
        }

        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is local only", $statement);
        }

        /**
         * Only dynamic variables and arrays can be used as arrays
         */
        if ($symbolVariable->isNotVariableAndArray()) {
            throw new CompilerException("Cannot use variable type: '" . $symbolVariable->getType() . "' as an array", $statement);
        }

        if ($symbolVariable->getType() == 'variable') {
            if ($symbolVariable->hasDifferentDynamicType(array('undefined', 'array'))) {
                $compilationContext->logger->warning('Possible attempt to append elements on a non-array dynamic variable', 'non-array-append', $statement);
            }
        }

        $codePrinter = $compilationContext->codePrinter;

        $compilationContext->headersManager->add('kernel/array');

        $type = $symbolVariable->getType();
        switch ($type) {
            case 'array':
            case 'variable':
                switch ($resolvedExpr->getType()) {
                    case 'null':
                        $codePrinter->output('zephir_array_append(&' . $variable . ', ZEPHIR_GLOBAL(global_null), PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                        $codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $resolvedExpr->getCode() . ');');
                        $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                        $symbolVariable->setIdle(true);
                        break;

                    case 'double':
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                        $codePrinter->output('ZVAL_DOUBLE(' . $symbolVariable->getName() . ', ' . $resolvedExpr->getCode() . ');');
                        $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                        $symbolVariable->setIdle(true);
                        break;

                    case 'bool':
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                        $codePrinter->output('ZVAL_BOOL(' . $symbolVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
                        $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                        $symbolVariable->setIdle(true);
                        break;

                    case 'ulong':
                    case 'string':
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                        $codePrinter->output('ZVAL_STRING(' . $symbolVariable->getName() . ', "' . Utils::addSlashes($resolvedExpr->getCode(), true) . '", 1);');
                        $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                        $symbolVariable->setIdle(true);
                        break;

                    case 'array':
                        $exprVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                        $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $exprVariable->getName() . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                        break;

                    case 'variable':
                        $exprVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                        switch ($exprVariable->getType()) {

                            case 'int':
                            case 'uint':
                            case 'long':
                                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                                $codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $exprVariable->getName() . ');');
                                $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                                $symbolVariable->setIdle(true);
                                break;

                            case 'double':
                                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                                $codePrinter->output('ZVAL_DOUBLE(' . $symbolVariable->getName() . ', ' . $exprVariable->getName() . ');');
                                $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                                $symbolVariable->setIdle(true);
                                break;

                            case 'bool':
                                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                                $codePrinter->output('ZVAL_BOOL(' . $symbolVariable->getName() . ', ' . $exprVariable->getName() . ');');
                                $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                                $symbolVariable->setIdle(true);
                                break;

                            case 'variable':
                            case 'string':
                            case 'array':
                                $codePrinter->output('zephir_array_append(&' . $variable . ', ' . $exprVariable->getName() . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                                break;

                            default:
                                throw new CompilerException("Unknown type: " . $exprVariable->getType(), $statement);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown type: " . $resolvedExpr->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException("Unknown type: " . $type, $statement);
        }
    }
}
