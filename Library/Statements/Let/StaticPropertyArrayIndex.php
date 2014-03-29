<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
 * StaticPropertyArrayIndex
 *
 * Updates object properties dynamically
 */
class StaticPropertyArrayIndex
{

	/**
     * Compiles x::y[z] = {expr} (single offset assignment)
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext,
     * @param array $statement
     */
    protected function _assignStaticPropertyArraySingleIndex($className, $property, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {

        $codePrinter = $compilationContext->codePrinter;

        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /**
         * Only string/variable/int
         */
        $indexExpression = new Expression($statement['index-expr'][0]);
        $resolvedIndex = $indexExpression->compile($compilationContext);

        switch ($resolvedIndex->getType()) {
            case 'string':
            case 'int':
            case 'uint':
            case 'ulong':
            case 'long':
            case 'variable':
                break;
            default:
                throw new CompilerException("Expression: " . $resolvedIndex->getType() . " cannot be used as index without cast", $statement['index-expr']);
        }

        if ($resolvedIndex->getType() == 'variable') {

            $indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedIndex->getCode(), $compilationContext, $statement['index-expr']);
            switch ($indexVariable->getType()) {
                case 'string':
                case 'int':
                case 'uint':
                case 'ulong':
                case 'long':
                case 'variable':
                    break;
                default:
                    throw new CompilerException("Variable: " . $indexVariable->getType() . " cannot be used as index without cast", $statement);
            }

            if ($indexVariable->getType() == 'variable') {
                if ($indexVariable->hasDifferentDynamicType(array('undefined', 'int', 'string'))) {
                    $compilationContext->logger->warning('Possible attempt to use non string/long dynamic variable as array index', 'invalid-array-offset', $statement);
                }
            }
        }

        switch ($resolvedIndex->getType()) {

            case 'string':
                $indexVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
                $codePrinter->output('ZVAL_STRING(' . $indexVariable->getName() . ', "' . $resolvedIndex->getCode() . '", 1);');
                break;

        }

        switch ($indexVariable->getType()) {

            case 'int':
                $codePrinter->output('//missing');
                break;

            case 'variable':
            case 'string':

                switch ($resolvedExpr->getType()) {

                    case 'null':
                        //$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
                        break;

                    case 'bool':
                        if ($resolvedExpr->getBooleanCode() == '1') {
                           // $codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ZEPHIR_GLOBAL(global_true) TSRMLS_CC);');
                        } else {
                            //$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
                        }
                        break;

                    case 'string':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $codePrinter->output('ZVAL_STRING(' . $tempVariable->getName() . ', "' . $resolvedExpr->getCode() . '", 1);');
                        //$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                        break;

                    case 'variable':
                        $variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement['index-expr']);
                        switch ($variableExpr->getType()) {

                            case 'bool':
                                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                                $codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $variableExpr->getName() . ');');
                                //$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
                                break;

                            case 'variable':
                            case 'string':
                                //$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $variableExpr->getName() . ' TSRMLS_CC);');
                                if ($variableExpr->isTemporal()) {
                                    $variableExpr->setIdle(true);
                                }
                                break;

                            default:
                                throw new CompilerException("Cannot update variable type: " . $variableExpr->getType(), $statement);

                        }
                        break;

                    default:
                        throw new CompilerException("Variable index: " . $indexVariable->getType() . " cannot be updated into array property", $statement);
                }
                break;

            default:
                throw new CompilerException("Index: " . $resolvedIndex->getType() . " cannot be updated into array property", $statement);
        }
    }

    /**
     * Compiles ClassName::foo[index] = {expr}
     *
     * @param                    $className
     * @param                    $property
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     * @internal param string $variable
     */
    public function assign($className, $property, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        /**
         * Update the property according to the number of array-offsets
         */
        if (count($statement['index-expr']) == 1) {
            $this->_assignStaticPropertyArraySingleIndex($className, $property, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
        } else {
            $this->_assignStaticPropertyArrayMultipleIndex($className, $property, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
        }
    }
}