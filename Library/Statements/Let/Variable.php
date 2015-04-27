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
 * Variable
 *
 * Assign a value to a variable
 */
class Variable
{
    private function doNumericAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext)
    {
        switch ($resolvedExpr->getType()) {

            case 'null':
                switch ($statement['operator']) {

                    case 'assign':
                        $codePrinter->output($variable . ' = 0;');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += 0;');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= 0;');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= 0;');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($statement['operator']) {

                    case 'assign':
                        $codePrinter->output($variable . ' = ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'div-assign':
                        $codePrinter->output($variable . ' /= ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'mod-assign':
                        $codePrinter->output($variable . ' %= ' . $resolvedExpr->getCode() . ';');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
                }
                break;

            case 'char':
            case 'uchar':
                switch ($statement['operator']) {

                    case 'assign':
                        $codePrinter->output($variable . ' = \'' . $resolvedExpr->getCode() . '\';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += \'' . $resolvedExpr->getCode() . '\';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= \'' . $resolvedExpr->getCode() . '\';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= \'' . $resolvedExpr->getCode() . '\';');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
                }
                break;

            case 'double':
                switch ($statement['operator']) {

                    case 'assign':
                        $codePrinter->output($variable . ' = (long) (' . $resolvedExpr->getCode() . ');');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += (long) (' . $resolvedExpr->getCode() . ');');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= (long) (' . $resolvedExpr->getCode() . ');');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= (long) (' . $resolvedExpr->getCode() . ');');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
                }
                break;

            case 'bool':
                switch ($statement['operator']) {

                    case 'assign':
                        $codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
                }
                break;

            case 'variable':

                $itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($itemVariable->getType()) {

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'bool':
                    case 'char':
                    case 'uchar':

                        switch ($statement['operator']) {

                            case 'assign':
                                $codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
                                break;

                            case 'add-assign':
                                $codePrinter->output($variable . ' += ' . $itemVariable->getName() . ';');
                                break;

                            case 'sub-assign':
                                $codePrinter->output($variable . ' -= ' . $itemVariable->getName() . ';');
                                break;

                            case 'mul-assign':
                                $codePrinter->output($variable . ' *= ' . $itemVariable->getName() . ';');
                                break;

                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
                        }
                        break;

                    case 'double':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable . ' = (long) ' . $itemVariable->getName() . ';');
                                break;
                            case 'add-assign':
                                $codePrinter->output($variable . ' += (long) ' . $itemVariable->getName() . ';');
                                break;
                            case 'sub-assign':
                                $codePrinter->output($variable . ' -= (long) ' . $itemVariable->getName() . ';');
                                break;
                            case 'mul-assign':
                                $codePrinter->output($variable . ' *= (long) ' . $itemVariable->getName() . ';');
                                break;
                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
                        }
                        break;

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        switch ($statement['operator']) {

                            case 'assign':
                                $codePrinter->output($variable . ' = zephir_get_numberval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
                                break;

                            case 'add-assign':
                                $codePrinter->output($variable . ' += zephir_get_numberval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
                                break;

                            case 'sub-assign':
                                $codePrinter->output($variable . ' -= zephir_get_numberval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
                                break;

                            case 'mul-assign':
                                $codePrinter->output($variable . ' *= zephir_get_numberval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
                                break;

                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown type: " . $itemVariable->getType(), $statement);
                }
                break;
            default:
                throw new CompilerException("Value type '" . $resolvedExpr->getType() . "' cannot be assigned to variable: int", $statement);
        }
    }

    private function doDoubleAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext)
    {
        switch ($resolvedExpr->getType()) {

            case 'null':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = 0.0;');
                        break;
                    case 'add-assign':
                        $codePrinter->output($variable . ' += 0.0;');
                        break;
                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= 0.0;');
                        break;
                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= 0.0;');
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($statement['operator']) {

                    case 'assign':
                        $codePrinter->output($variable . ' = (double) (' . $resolvedExpr->getCode() . ');');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += (double) (' . $resolvedExpr->getCode() . ');');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= (double) (' . $resolvedExpr->getCode() . ');');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= (double) (' . $resolvedExpr->getCode() . ');');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
                }
                break;

            case 'double':
                switch ($statement['operator']) {

                    case 'assign':
                        $codePrinter->output($variable . ' = ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= ' . $resolvedExpr->getCode() . ';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= ' . $resolvedExpr->getCode() . ';');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
                }
                break;

            case 'bool':
                switch ($statement['operator']) {

                    case 'assign':
                        $codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    case 'add-assign':
                        $codePrinter->output($variable . ' += ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    case 'sub-assign':
                        $codePrinter->output($variable . ' -= ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    case 'mul-assign':
                        $codePrinter->output($variable . ' *= ' . $resolvedExpr->getBooleanCode() . ';');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($itemVariable->getType()) {

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'bool':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable . ' = (double) ' . $itemVariable->getName() . ';');
                                break;
                            case 'add-assign':
                                $codePrinter->output($variable . ' += (double) ' . $itemVariable->getName() . ';');
                                break;
                            case 'sub-assign':
                                $codePrinter->output($variable . ' -= (double) ' . $itemVariable->getName() . ';');
                                break;
                            case 'mul-assign':
                                $codePrinter->output($variable . ' *= (double) ' . $itemVariable->getName() . ';');
                                break;
                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
                        }
                        break;

                    case 'double':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
                                break;
                            case 'add-assign':
                                $codePrinter->output($variable . ' += ' . $itemVariable->getName() . ';');
                                break;
                            case 'sub-assign':
                                $codePrinter->output($variable . ' -= ' . $itemVariable->getName() . ';');
                                break;
                            case 'mul-assign':
                                $codePrinter->output($variable . ' *= ' . $itemVariable->getName() . ';');
                                break;
                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
                        }
                        break;

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable . ' = zephir_get_numberval(' . $itemVariable->getName() . ');');
                                break;
                            case 'add-assign':
                                $codePrinter->output($variable . ' += zephir_get_numberval(' . $itemVariable->getName() . ');');
                                break;
                            case 'sub-assign':
                                $codePrinter->output($variable . ' -= zephir_get_numberval(' . $itemVariable->getName() . ');');
                                break;
                            case 'mul-assign':
                                $codePrinter->output($variable . ' *= zephir_get_numberval(' . $itemVariable->getName() . ');');
                                break;
                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown type: " . $itemVariable->getType(), $statement);
                }
                break;
            default:
                throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
        }
    }

    private function doStringAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext)
    {
        switch ($resolvedExpr->getType()) {

            case 'null':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        $codePrinter->output('ZVAL_EMPTY_STRING(' . $variable . ');');
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        $codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
                        break;
                    case 'concat-assign':
                        $codePrinter->output('zephir_concat_self_str(&' . $variable . ', "' . $resolvedExpr->getCode() . '", sizeof("' . $resolvedExpr->getCode() . '")-1 TSRMLS_CC);');
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
                }
                break;

            case 'string':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        if ($resolvedExpr->getCode()) {
                            $codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
                        } else {
                            $codePrinter->output('ZVAL_EMPTY_STRING(' . $variable . ');');
                        }
                        break;
                    case 'concat-assign':
                        $codePrinter->output('zephir_concat_self_str(&' . $variable . ', "' . $resolvedExpr->getCode() . '", sizeof("' . $resolvedExpr->getCode() . '")-1 TSRMLS_CC);');
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
                }
                break;

            case 'char':
            case 'uchar':
                switch ($statement['operator']) {

                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        if ($resolvedExpr->getCode()) {
                            $codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
                        } else {
                            $codePrinter->output('ZVAL_EMPTY_STRING(' . $variable . ');');
                        }
                        break;

                    case 'concat-assign':
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output('zephir_concat_self_str(&' . $variable . ', "' . $resolvedExpr->getCode() . '", sizeof("' . $resolvedExpr->getCode() . '")-1 TSRMLS_CC);');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($itemVariable->getType()) {

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        switch ($statement['operator']) {

                            case 'assign':
                                $symbolVariable->initVariant($compilationContext);
                                $compilationContext->headersManager->add('kernel/string');
                                $codePrinter->output('Z_STRLEN_P(' . $variable . ') = zephir_spprintf(&Z_STRVAL_P(' . $variable . '), 0, "%ld", ' . $itemVariable->getName() . ');');
                                $codePrinter->output('Z_TYPE_P(' . $variable . ') = IS_STRING;');
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('zephir_concat_self_long(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
                                break;

                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
                        }
                        break;

                    case 'char':
                    case 'uchar':
                        switch ($statement['operator']) {

                            case 'assign':
                                $symbolVariable->initVariant($compilationContext);
                                $compilationContext->headersManager->add('kernel/string');
                                $codePrinter->output('Z_STRLEN_P(' . $variable . ') = zephir_spprintf(&Z_STRVAL_P(' . $variable . '), 0, "%c", ' . $itemVariable->getName() . ');');
                                $codePrinter->output('Z_TYPE_P(' . $variable . ') = IS_STRING;');
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('zephir_concat_self_char(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
                                break;

                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
                        }
                        break;

                    case 'string':
                        switch ($statement['operator']) {

                            case 'assign':
                                $symbolVariable->setMustInitNull(true);
                                $symbolVariable->increaseVariantIfNull();
                                $compilationContext->symbolTable->mustGrownStack(true);
                                if ($variable != $itemVariable->getName()) {
                                    $codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $itemVariable->getName() . ');');
                                }
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('zephir_concat_self(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
                                break;

                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
                        }
                        break;

                    case 'variable':
                        switch ($statement['operator']) {

                            case 'assign':
                                $symbolVariable->setMustInitNull(true);
                                $compilationContext->symbolTable->mustGrownStack(true);
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('zephir_get_strval(' . $variable . ', ' . $itemVariable->getName() . ');');
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('zephir_concat_self(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
                                break;

                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown type: " . $itemVariable->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
        }
    }

    private function doArrayAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext)
    {
        switch ($resolvedExpr->getType()) {

            case 'variable':
            case 'array':
                switch ($statement['operator']) {
                    case 'assign':

                        if ($variable != $resolvedExpr->getCode()) {
                            $symbolVariable->setMustInitNull(true);
                            $compilationContext->symbolTable->mustGrownStack(true);

                            /* Inherit the dynamic type data from the assigned value */
                            $symbolVariable->setDynamicTypes('array');
                            $symbolVariable->increaseVariantIfNull();

                            $codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $resolvedExpr->getCode() . ');');
                        }
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
                }
                break;

            default:
                throw new CompilerException("Cannot '" . $statement['operator'] . "' " . $resolvedExpr->getType() . " for array type", $resolvedExpr->getOriginal());
        }
    }

    private function doBoolAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext)
    {
        switch ($resolvedExpr->getType()) {
            case 'null':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = 0;');
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: null", $statement);
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = (' . $resolvedExpr->getCode() . ') ? 1 : 0;');
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $statement);
                }
                break;

            case 'char':
            case 'uchar':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = (\'' . $resolvedExpr->getCode() . '\') ? 1 : 0;');
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $statement);
                }
                break;

            case 'bool':
                switch ($statement['operator']) {
                    case 'assign':
                        $codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $statement);
                }
                break;

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($itemVariable->getType()) {

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'double':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable . ' = (' . $itemVariable->getName() . ') ? 1 : 0;');
                                break;
                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
                        }
                        break;

                    case 'bool':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
                                break;
                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
                        }
                        break;

                    case 'variable':
                    case 'string':
                    case 'array':
                        switch ($statement['operator']) {
                            case 'assign':
                                $codePrinter->output($variable . ' = zephir_is_true(' . $itemVariable->getName() . ');');
                                break;
                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot assign variable: " . $itemVariable->getType(), $statement);
                }
                break;

            default:
                throw new CompilerException("Unknown type: " . $resolvedExpr->getType(), $statement);
        }
    }

    private function doVariableAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext, $readDetector)
    {
        switch ($resolvedExpr->getType()) {

            case 'null':
                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        $symbolVariable->setDynamicTypes('null');

                        if ($symbolVariable->isLocalOnly()) {
                            $codePrinter->output('ZVAL_NULL(&' . $variable . ');');
                        } else {
                            $codePrinter->output('ZVAL_NULL(' . $variable . ');');
                        }
                        break;
                }
                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':

                if ($symbolVariable->isLocalOnly()) {
                    $symbol = '&' . $variable;
                } else {
                    $symbol = $variable;
                }

                switch ($statement['operator']) {

                    case 'mul-assign':
                    case 'sub-assign':
                    case 'add-assign':

                        switch ($statement['operator']) {

                            case 'mul-assign':
                                $functionName = 'ZEPHIR_MUL_ASSIGN';
                                break;

                            case 'sub-assign':
                                $functionName = 'ZEPHIR_SUB_ASSIGN';
                                break;

                            case 'add-assign':
                                $functionName = 'ZEPHIR_ADD_ASSIGN';
                                break;
                        }


                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_LONG(' . $tempVariable->getName(). ', ' . $resolvedExpr->getCode() . ');');

                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output($functionName . '(' . $symbol . ', ' . $tempVariable->getName() . ');');
                        break;

                    case 'assign':
                        $symbolVariable->setDynamicTypes('long');
                        if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('int', $compilationContext);
                            $codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getCode() . ';');
                            $symbolVariable->initVariant($compilationContext);
                            $codePrinter->output('ZVAL_LONG(' . $symbol . ', ' . $tempVariable->getName() . ');');
                        } else {
                            $symbolVariable->initVariant($compilationContext);
                            $codePrinter->output('ZVAL_LONG(' . $symbol . ', ' . $resolvedExpr->getCode() . ');');
                        }
                        break;

                    case 'div-assign':
                        $symbolVariable->setDynamicTypes('double');
                        if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('double', $compilationContext);
                            $codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getCode() . ';');
                            $symbolVariable->initVariant($compilationContext);
                            $codePrinter->output('ZVAL_DOUBLE(' . $symbol . ', ' . $tempVariable->getName() . ');');
                        } else {
                            $symbolVariable->initVariant($compilationContext);
                            $codePrinter->output('ZVAL_DOUBLE(' . $symbol . ', ' . $resolvedExpr->getCode() . ');');
                        }
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
                }
                break;

            case 'char':
            case 'uchar':

                if ($symbolVariable->isLocalOnly()) {
                    $symbol = '&' . $variable;
                } else {
                    $symbol = $variable;
                }

                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->setDynamicTypes('long');
                        if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('char', $compilationContext);
                            $codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getCode() . ';');
                            $symbolVariable->initVariant($compilationContext);
                            $codePrinter->output('ZVAL_LONG(' . $symbol . ', ' . $tempVariable->getName() . ');');
                        } else {
                            $symbolVariable->initVariant($compilationContext);
                            $codePrinter->output('ZVAL_LONG(' . $symbol . ', \'' . $resolvedExpr->getCode() . '\');');
                        }
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
                }
                break;

            case 'double':

                if ($symbolVariable->isLocalOnly()) {
                    $symbol = '&' . $variable;
                } else {
                    $symbol = $variable;
                }

                switch ($statement['operator']) {
                    case 'assign':
                        $symbolVariable->setDynamicTypes('double');
                        if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                            $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('double', $compilationContext);
                            $codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getCode() . ';');
                            $symbolVariable->initVariant($compilationContext);
                            $codePrinter->output('ZVAL_DOUBLE(' . $symbol . ', ' . $tempVariable->getName() . ');');
                        } else {
                            $symbolVariable->initVariant($compilationContext);
                            $codePrinter->output('ZVAL_DOUBLE(' . $symbol . ', ' . $resolvedExpr->getCode() . ');');
                        }
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
                }
                break;

            case 'bool':

                if ($symbolVariable->isLocalOnly()) {
                    $symbol = '&' . $variable;
                } else {
                    $symbol = $variable;
                }

                switch ($statement['operator']) {

                    case 'assign':
                        $symbolVariable->setDynamicTypes('bool');
                        if ($resolvedExpr->getCode() == 'true') {
                            $symbolVariable->initVariant($compilationContext);
                            $codePrinter->output('ZVAL_BOOL(' . $symbol . ', 1);');
                        } else {
                            if ($resolvedExpr->getCode() == 'false') {
                                $symbolVariable->initVariant($compilationContext);
                                $codePrinter->output('ZVAL_BOOL(' . $symbol . ', 0);');
                            } else {
                                if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
                                    $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('double', $compilationContext);
                                    $codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getBooleanCode() . ';');
                                    $symbolVariable->initVariant($compilationContext);
                                    $codePrinter->output('ZVAL_BOOL(' . $symbol . ', ' . $tempVariable->getName() . ');');
                                } else {
                                    $symbolVariable->initVariant($compilationContext);
                                    $codePrinter->output('ZVAL_BOOL(' . $symbol . ', ' . $resolvedExpr->getBooleanCode() . ');');
                                }
                            }
                        }
                        break;
                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
                }
                break;

            case 'string':
                switch ($statement['operator']) {

                    case 'assign':
                        $symbolVariable->initVariant($compilationContext);
                        $symbolVariable->setDynamicTypes('string');
                        if ($symbolVariable->isLocalOnly()) {
                            $codePrinter->output('ZVAL_STRING(&' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
                        } else {
                            $codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
                        }
                        break;

                    case 'concat-assign':
                        $compilationContext->headersManager->add('kernel/operators');
                        $codePrinter->output('zephir_concat_self_str(&' . $variable . ', SL("' . $resolvedExpr->getCode() . '") TSRMLS_CC);');
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
                }
                break;

            case 'array':
                switch ($statement['operator']) {
                    case 'assign':

                        if ($variable != $resolvedExpr->getCode()) {
                            $symbolVariable->setMustInitNull(true);
                            $compilationContext->symbolTable->mustGrownStack(true);

                            /* Inherit the dynamic type data from the assigned value */
                            $symbolVariable->setDynamicTypes('array');
                            $symbolVariable->increaseVariantIfNull();

                            $codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $resolvedExpr->getCode() . ');');
                        }
                        break;

                    default:
                        throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
                }
                break;

            case 'variable':

                $itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $resolvedExpr->getOriginal());
                switch ($itemVariable->getType()) {

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'char':
                    case 'uchar':
                        switch ($statement['operator']) {

                            case 'assign':
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('long');
                                if ($symbolVariable->isLocalOnly()) {
                                    $codePrinter->output('ZVAL_LONG(&' . $variable . ', ' . $itemVariable->getName() . ');');
                                } else {
                                    $codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $itemVariable->getName() . ');');
                                }
                                break;

                            case 'add-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('long');
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite($itemVariable->getType(), $compilationContext);
                                if ($symbolVariable->isLocalOnly()) {
                                    $codePrinter->output($tempVariable->getName() . ' = zephir_get_numberval(&' . $variable . ');');
                                    $codePrinter->output('ZVAL_LONG(&' . $variable . ', ' . $tempVariable->getName() . ' + ' . $itemVariable->getName() . ');');
                                } else {
                                    $codePrinter->output($tempVariable->getName() . ' = zephir_get_numberval(' . $variable . ');');
                                    $codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $tempVariable->getName() . ' + ' . $itemVariable->getName() . ');');
                                }
                                break;

                            case 'sub-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('long');
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite($itemVariable->getType(), $compilationContext);
                                if ($symbolVariable->isLocalOnly()) {
                                    $codePrinter->output($tempVariable->getName() . ' = zephir_get_numberval(&' . $variable . ');');
                                    $codePrinter->output('ZVAL_LONG(&' . $variable . ', ' . $tempVariable->getName() . ' - ' . $itemVariable->getName() . ');');
                                } else {
                                    $codePrinter->output($tempVariable->getName() . ' = zephir_get_numberval(' . $variable . ');');
                                    $codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $tempVariable->getName() . ' - ' . $itemVariable->getName() . ');');
                                }
                                break;
                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
                        }
                        break;

                    case 'double':
                        switch ($statement['operator']) {
                            case 'assign':
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('double');
                                if ($symbolVariable->isLocalOnly()) {
                                    $codePrinter->output('ZVAL_DOUBLE(&' . $variable . ', ' . $itemVariable->getName() . ');');
                                } else {
                                    $codePrinter->output('ZVAL_DOUBLE(' . $variable . ', ' . $itemVariable->getName() . ');');
                                }
                                break;
                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
                        }
                        break;

                    case 'bool':
                        switch ($statement['operator']) {
                            case 'assign':
                                $symbolVariable->initVariant($compilationContext);
                                $symbolVariable->setDynamicTypes('bool');
                                if ($symbolVariable->isLocalOnly()) {
                                    $codePrinter->output('ZVAL_BOOL(&' . $variable . ', ' . $itemVariable->getName() . ');');
                                } else {
                                    $codePrinter->output('ZVAL_BOOL(' . $variable . ', ' . $itemVariable->getName() . ');');
                                }
                                break;
                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
                        }
                        break;

                    case 'array':
                        switch ($statement['operator']) {
                            case 'assign':

                                if ($variable != $resolvedExpr->getCode()) {
                                    $symbolVariable->setMustInitNull(true);
                                    $compilationContext->symbolTable->mustGrownStack(true);

                                    /* Inherit the dynamic type data from the assigned value */
                                    $symbolVariable->setDynamicTypes('array');
                                    $symbolVariable->increaseVariantIfNull();

                                    $codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $resolvedExpr->getCode() . ');');
                                }
                                break;

                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
                        }
                        break;

                    case 'variable':

                        switch ($statement['operator']) {
                            case 'assign':
                                if ($itemVariable->getName() != $variable) {
                                    $symbolVariable->setMustInitNull(true);
                                    $compilationContext->symbolTable->mustGrownStack(true);

                                    /* Inherit the dynamic type data from the assigned value */
                                    $symbolVariable->setDynamicTypes(array_keys($itemVariable->getDynamicTypes()));

                                    $symbolVariable->setClassTypes($itemVariable->getClassTypes());
                                    $symbolVariable->increaseVariantIfNull();

                                    $codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $itemVariable->getName() . ');');
                                    if ($itemVariable->isTemporal()) {
                                        $itemVariable->setIdle(true);
                                    }
                                }
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('zephir_concat_self(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
                                break;

                            case 'add-assign':
                                $compilationContext->symbolTable->mustGrownStack(true);
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('ZEPHIR_ADD_ASSIGN(' . $variable . ', ' . $itemVariable->getName() . ');');
                                break;

                            case 'sub-assign':
                                $compilationContext->symbolTable->mustGrownStack(true);
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('ZEPHIR_SUB_ASSIGN(' . $variable . ', ' . $itemVariable->getName() . ');');
                                break;

                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
                        }
                        break;

                    case 'string':
                        switch ($statement['operator']) {

                            case 'assign':

                                if ($itemVariable->getName() != $variable) {
                                    $symbolVariable->setMustInitNull(true);
                                    $compilationContext->symbolTable->mustGrownStack(true);

                                    /* Inherit the dynamic type data from the assigned value */
                                    $symbolVariable->setDynamicTypes(array_keys($itemVariable->getDynamicTypes()));
                                    $symbolVariable->setClassTypes($itemVariable->getClassTypes());
                                    $symbolVariable->increaseVariantIfNull();

                                    $codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $itemVariable->getName() . ');');
                                    if ($itemVariable->isTemporal()) {
                                        $itemVariable->setIdle(true);
                                    }
                                }
                                break;

                            case 'concat-assign':
                                $compilationContext->headersManager->add('kernel/operators');
                                $codePrinter->output('zephir_concat_self(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
                                break;

                            default:
                                throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
                        }
                        break;

                    default:
                        throw new CompilerException("Unknown type: " . $itemVariable->getType(), $resolvedExpr->getOriginal());
                }
                break;

            default:
                throw new CompilerException("Unknown type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
        }
    }

    /**
     * Compiles foo = {expr}
     * Changes the value of a mutable variable
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param ReadDetector $readDetector
     * @param CompilationContext $compilationContext
     * @param array $statement
     * @throws CompilerException
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, ReadDetector $readDetector, CompilationContext $compilationContext, array $statement)
    {
        if ($symbolVariable->isReadOnly()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is read only", $statement);
        }

        $codePrinter = $compilationContext->codePrinter;

        /**
         * Only initialize variables if it's direct assignment
         */
        if ($statement['operator'] == 'assign') {
            $symbolVariable->setIsInitialized(true, $compilationContext, $statement);
        } else {
            if (!$symbolVariable->isInitialized()) {
                throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is not initialized", $statement);
            }
        }

        /**
         * Set the assigned value to the variable as a CompiledExpression
         * We could use this expression for further analysis
         */
        $symbolVariable->setPossibleValue($resolvedExpr, $compilationContext);

        $type = $symbolVariable->getType();
        switch ($type) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'char':
            case 'uchar':
                $this->doNumericAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext);
                break;

            case 'double':
                $this->doDoubleAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext);
                break;

            case 'array':
                $this->doArrayAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext);
                break;

            case 'string':
                $this->doStringAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext);
                break;

            case 'bool':
                $this->doBoolAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext);
                break;

            case 'variable':
                $this->doVariableAssignment($codePrinter, $resolvedExpr, $symbolVariable, $variable, $statement, $compilationContext, $readDetector);
                break;

            default:
                throw new CompilerException("Unknown type: " . $type, $statement);
        }
    }
}
