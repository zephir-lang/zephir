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

namespace Zephir\Expression;

use Zephir\Exception;
use Zephir\Variable;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\CompilerException;
use Zephir\Expression;
use Zephir\Compiler;

/**
 * Reference
 *
 * Resolves expressions that create arrays
 */
class Reference
{
    protected $_expecting = true;

    protected $_readOnly = false;

    protected $_expectingVariable;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value
     *
     * @param boolean $expecting
     * @param Variable $expectingVariable
     */
    public function setExpectReturn($expecting, Variable $expectingVariable = null)
    {
        $this->_expecting = $expecting;
        $this->_expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only
     *
     * @param boolean $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->_readOnly = $readOnly;
    }

    /**
     * Resolves an item to be added in an array
     *
     * @param CompiledExpression $exprCompiled
     * @param CompilationContext $compilationContext
     * @return Variable
     */
    public function getArrayValue($exprCompiled, CompilationContext $compilationContext)
    {
        $codePrinter = $compilationContext->codePrinter;

        switch ($exprCompiled->getType()) {

            case 'int':
            case 'uint':
            case 'long':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('ZVAL_LONG(' . $tempVar->getName() . ', ' . $exprCompiled->getCode() . ');');
                return $tempVar;

            case 'char':
            case 'uchar':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('ZVAL_LONG(' . $tempVar->getName() . ', \'' . $exprCompiled->getCode() . '\');');
                return $tempVar;

            case 'double':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('ZVAL_DOUBLE(' . $tempVar->getName() . ', ' . $exprCompiled->getCode() . ');');
                return $tempVar;

            case 'bool':
                if ($exprCompiled->getCode() == 'true') {
                    return new GlobalConstant('ZEPHIR_GLOBAL(global_true)');
                } else {
                    if ($exprCompiled->getCode() == 'false') {
                        return new GlobalConstant('ZEPHIR_GLOBAL(global_false)');
                    } else {
                        throw new Exception('?');
                    }
                }
                break;

            case 'null':
                return new GlobalConstant('ZEPHIR_GLOBAL(global_null)');

            case 'string':
            case 'ulong':
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $codePrinter->output('ZVAL_STRING(' . $tempVar->getName() . ', "' . $exprCompiled->getCode() . '", 1);');
                return $tempVar;

            case 'array':
                return $compilationContext->symbolTable->getVariableForRead($exprCompiled->getCode(), $compilationContext, $exprCompiled->getOriginal());

            case 'variable':
                $itemVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiled->getCode(), $compilationContext, $exprCompiled->getOriginal());
                switch ($itemVariable->getType()) {

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_LONG(' . $tempVar->getName() . ', ' . $itemVariable->getName() . ');');
                        return $tempVar;

                    case 'double':
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_DOUBLE(' . $tempVar->getName() . ', ' . $itemVariable->getName() . ');');
                        return $tempVar;

                    case 'bool':
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_BOOL(' . $tempVar->getName() . ', ' . $itemVariable->getName() . ');');
                        return $tempVar;

                    case 'string':
                    case 'variable':
                    case 'array':
                        return $itemVariable;

                    default:
                        throw new CompilerException("Unknown " . $itemVariable->getType(), $itemVariable);
                }
                break;

            default:
                throw new CompilerException("Unknown", $exprCompiled);
        }
    }

    /**
     * Compiles a reference to a value
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     */
    public function compile($expression, CompilationContext $compilationContext)
    {

        /**
         * Resolves the symbol that expects the value
         */
        if ($this->_expecting) {
            if ($this->_expectingVariable) {
                $symbolVariable = $this->_expectingVariable;
                if ($symbolVariable->getType() != 'variable') {
                    throw new CompilerException("Cannot use variable type: " . $symbolVariable->getType() . " to store a reference", $expression);
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->_readOnly);
        $left = $leftExpr->compile($compilationContext);

        switch ($left->getType()) {
            case 'variable':
            case 'string':
            case 'object':
            case 'array':
            case 'callable':
                break;
            default:
                throw new CompilerException("Cannot obtain a reference from type: " . $left->getType(), $expression);
        }

        $leftVariable = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression);
        switch ($leftVariable->getType()) {
            case 'variable':
            case 'string':
            case 'object':
            case 'array':
            case 'callable':
                break;
            default:
                throw new CompilerException("Cannot obtain reference from variable type: " . $leftVariable->getType(), $expression);
        }

        $symbolVariable->setMustInitNull(true);
        $compilationContext->symbolTable->mustGrownStack(true);

        $symbolVariable->increaseVariantIfNull();
        $compilationContext->codePrinter->output('ZEPHIR_MAKE_REFERENCE(' . $symbolVariable->getName() . ', ' . $leftVariable->getName() . ');');

        return new CompiledExpression('reference', $symbolVariable->getRealName(), $expression);
    }
}
