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
 * ArrayIndex
 *
 * Adds/Updates an array index
 */
class ArrayIndex
{

    /**
     * Resolves an item that will be assigned to an array offset
     *
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     */
    protected function _getResolvedArrayItem(CompiledExpression $resolvedExpr, CompilationContext $compilationContext)
    {
        $codePrinter = $compilationContext->codePrinter;

        switch ($resolvedExpr->getType()) {

            case 'null':
                $symbolVariable = new GlobalConstant('ZEPHIR_GLOBAL(global_null)');
                break;

            case 'int':
            case 'uint':
            case 'long':
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                $codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $resolvedExpr->getCode() . ');');
                break;

            case 'char':
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                $codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', \'' . $resolvedExpr->getCode() . '\');');
                break;

            case 'double':
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                $codePrinter->output('ZVAL_DOUBLE(' . $symbolVariable->getName() . ', ' . $resolvedExpr->getCode() . ');');
                break;

            case 'bool':
                if ($resolvedExpr->getBooleanCode() == '1') {
                    $symbolVariable = new GlobalConstant('ZEPHIR_GLOBAL(global_true)');
                } else {
                    if ($resolvedExpr->getBooleanCode() == '0') {
                        $symbolVariable = new GlobalConstant('ZEPHIR_GLOBAL(global_false)');
                    } else {
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                        $codePrinter->output('ZVAL_BOOL(' . $symbolVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
                    }
                }
                break;

            case 'string':
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                $codePrinter->output('ZVAL_STRING(' . $symbolVariable->getName() . ', "' . $resolvedExpr->getCode() . '", 1);');
                break;

            case 'array':
                $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $resolvedExpr->getOriginal());
                break;

            case 'variable':
                $variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $resolvedExpr->getOriginal());
                switch ($variableExpr->getType()) {

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                        $codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $variableExpr->getName() . ');');
                        break;

                    case 'double':
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                        $codePrinter->output('ZVAL_DOUBLE(' . $symbolVariable->getName() . ', ' . $variableExpr->getName() . ');');
                        break;

                    case 'bool':
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
                        $codePrinter->output('ZVAL_BOOL(' . $symbolVariable->getName() . ', ' . $variableExpr->getName() . ');');
                        break;

                    case 'variable':
                    case 'string':
                    case 'array':
                        $symbolVariable = $variableExpr;
                        break;

                    default:
                        throw new CompilerException("Variable: " . $variableExpr->getType() . " cannot be assigned to array offset", $resolvedExpr->getOriginal());
                }
                break;

            default:
                throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be assigned to array offset", $resolvedExpr->getOriginal());
        }

        return $symbolVariable;
    }

    /**
     * Compiles foo[y] = {expr} (one offset)
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array $statement
     */
    protected function _assignArrayIndexSingle($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        $expression = new Expression($statement['index-expr'][0]);
        $exprIndex = $expression->compile($compilationContext);

        switch ($exprIndex->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'string':
            case 'variable':
                break;
            default:
                throw new CompilerException("Index: " . $exprIndex->getType() . " cannot be used as array offset in assignment without cast", $statement['index-expr'][0]);
        }

        $codePrinter = $compilationContext->codePrinter;

        /**
         * Create a temporal zval (if needed)
         */
        $symbolVariable = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

        $flags = 'PH_COPY | PH_SEPARATE';
        $isGlobalVariable = $compilationContext->symbolTable->isSuperGlobal($variable);

        $compilationContext->headersManager->add('kernel/array');

        if ($isGlobalVariable) {
            $variableTempSeparated = $compilationContext->symbolTable->getTempLocalVariableForWrite('int', $compilationContext);
            $codePrinter->output($variableTempSeparated->getName().' = zephir_maybe_separate_zval(&' . $variable . ');');
        }
        
        switch ($exprIndex->getType()) {

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                $codePrinter->output('zephir_array_update_long(&' . $variable . ', ' . $exprIndex->getCode() . ', &' . $symbolVariable->getName() . ', ' . $flags . ', "' . Compiler::getShortUserPath($statement['index-expr'][0]['file']) . '", ' . $statement['index-expr'][0]['line'] . ');');
                break;

            case 'string':
                $codePrinter->output('zephir_array_update_string(&' . $variable . ', SL("' . $exprIndex->getCode() . '"), &' . $symbolVariable->getName() . ', ' . $flags . ');');
                break;

            case 'variable':
                $variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $compilationContext, $statement);
                $variableIndexName = ($variableIndex->isLocalOnly() ? '&' : '') . $variableIndex->getName();

                switch ($variableIndex->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $codePrinter->output('zephir_array_update_long(&' . $variable . ', ' . $variableIndexName . ', &' . $symbolVariable->getName() . ', ' . $flags . ', "' . Compiler::getShortUserPath($statement['index-expr'][0]['file']) . '", ' . $statement['index-expr'][0]['line'] . ');');
                        break;
                    case 'string':
                        $codePrinter->output('zephir_array_update_zval(&' . $variable . ', ' . $variableIndexName . ', &' . $symbolVariable->getName() . ', ' . $flags . ');');
                        break;
                    case 'variable':
                        $codePrinter->output('zephir_array_update_zval(&' . $variable . ', ' . $variableIndexName . ', &' . $symbolVariable->getName() . ', ' . $flags . ');');
                        break;
                    default:
                        throw new CompilerException("Variable: " . $variableIndex->getType() . " cannot be used as array index", $statement);
                }
                break;
            default:
                throw new CompilerException("Value: " . $exprIndex->getType() . " cannot be used as array index", $statement);
        }
        
        if ($isGlobalVariable) {
            $codePrinter->output('if (' . $variableTempSeparated->getName() . ') {');
            $codePrinter->output("\t" . 'ZEND_SET_SYMBOL(&EG(symbol_table), "' . $variable . '", ' . $variable . ');');
            $codePrinter->output('}');
        }
    }

    /**
     * Compiles foo[y][x] = {expr} (multiple offset)
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array $statement
     */
    protected function _assignArrayIndexMultiple($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        $codePrinter = $compilationContext->codePrinter;

        $offsetExprs = array();
        foreach ($statement['index-expr'] as $indexExpr) {
            $expression = new Expression($indexExpr);
            $expression->setReadOnly(true);
            $exprIndex = $expression->compile($compilationContext);

            switch ($exprIndex->getType()) {
                case 'int':
                case 'uint':
                case 'long':
                case 'ulong':
                case 'string':
                case 'variable':
                    break;
                default:
                    throw new CompilerException("Index: " . $exprIndex->getType() . " cannot be used as array index in assignment without cast", $indexExpr);
            }

            $offsetExprs[] = $exprIndex;
        }

        $compilationContext->headersManager->add('kernel/array');

        /**
         * Create a temporal zval (if needed)
         */
        $symbolVariable = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

        $keys = '';
        $numberParams = 0;
        $offsetItems = array();
        foreach ($offsetExprs as $offsetExpr) {
            switch ($offsetExpr->getType()) {

                case 'int':
                case 'uint':
                case 'long':
                case 'ulong':
                    $keys .= 'l';
                    $offsetItems[] = $offsetExpr->getCode();
                    $numberParams++;
                    break;

                case 'string':
                    $keys .= 's';
                    $offsetItems[] = 'SL("' . $offsetExpr->getCode() . '")';
                    $numberParams += 2;
                    break;

                case 'variable':
                    $variableIndex = $compilationContext->symbolTable->getVariableForRead($offsetExpr->getCode(), $compilationContext, $statement);
                    switch ($variableIndex->getType()) {
                        case 'int':
                        case 'uint':
                        case 'long':
                        case 'ulong':
                            $keys .= 'l';
                            $offsetItems[] = $variableIndex->getName();
                            $numberParams++;
                            break;
                        case 'string':
                        case 'variable':
                            $keys .= 'z';
                            $offsetItems[] = $variableIndex->getName();
                            $numberParams++;
                            break;
                        default:
                            throw new CompilerException("Variable: " . $variableIndex->getType() . " cannot be used as array index", $statement);
                    }
                    break;

                default:
                    throw new CompilerException("Value: " . $offsetExpr->getType() . " cannot be used as array index", $statement);
            }
        }

        $codePrinter->output('zephir_array_update_multi(&' . $variable . ', &' . $symbolVariable->getName() . ' TSRMLS_CC, SL("' . $keys . '"), ' . $numberParams . ', ' . join(', ', $offsetItems) . ');');

        if ($symbolVariable->isTemporal()) {
            $symbolVariable->setIdle(true);
        }
    }

    /**
     * Compiles foo[y] = {expr}
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array $statement
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {

        /**
         * Arrays must be stored in the HEAP
         */
        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is local only", $statement);
        }

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
         * Only dynamic variables can be used as arrays
         */
        if ($symbolVariable->isNotVariableAndArray()) {
            throw new CompilerException("Cannot use variable type: '" . $symbolVariable->getType() . "' as array", $statement);
        }

        if ($symbolVariable->getType() == 'variable') {
            if ($symbolVariable->hasAnyDynamicType('unknown')) {
                throw new CompilerException("Cannot use non-initialized variable as an object", $statement);
            }

            /**
             * Trying to use a non-object dynamic variable as object
             */
            if ($symbolVariable->hasDifferentDynamicType(array('undefined', 'array', 'null'))) {
                $compilationContext->logger->warning('Possible attempt to update index on a non-array dynamic variable', 'non-array-update', $statement);
            }
        }

        /**
         * Choose one-offset or multiple-offset functions
         */
        if (count($statement['index-expr']) == 1) {
            $this->_assignArrayIndexSingle($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
        } else {
            $this->_assignArrayIndexMultiple($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
        }
    }
}
