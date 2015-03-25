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

namespace Zephir\Operators\Other;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompiledExpression;

/**
 * FetchOperator
 *
 * Fetch is a special operator that checks if an expression 'isset' and then obtain the value
 * without calculating the hash key twice
 */
class FetchOperator extends BaseOperator
{

    /**
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/array');

        $variable = $compilationContext->symbolTable->getVariableForWrite($expression['left']['value'], $compilationContext, $expression['left']);
        if ($variable->getType() != 'variable') {
            throw new CompilerException('Cannot use variable type: ' . $variable->gettype() . ' in "fetch" operator', $expression);
        }

        /**
         * return_value must not be observed
         */
        if ($variable->getName() != 'return_value') {
            /*
             * @todo use a read detector here
             */
            $readOnly = false;
            $line = max($compilationContext->symbolTable->getLastCallLine(), $compilationContext->symbolTable->getLastUnsetLine());
            if ($line === false || ($line > 0 && $line < $expression['line'])) {
                $numberMutations = $compilationContext->symbolTable->getExpectedMutations($variable->getName());
                if ($numberMutations == 1) {
                    if ($variable->getNumberMutations() == 1) {
                        $variable->setIsInitialized(true, $compilationContext, $expression);
                        $variable->setMemoryTracked(false);
                        $variable->setDynamicTypes('undefined');
                        $readOnly = true;
                    }
                }
            }

            if (!$readOnly || $expression['right']['type'] != 'array-access') {
                $variable->setIsInitialized(true, $compilationContext, $expression);
                $variable->observeVariant($compilationContext);
                $variable->setDynamicTypes('undefined');
                $variable->setPossibleValue(new CompiledExpression('undefined', '', $expression), $compilationContext);
            }
        } else {
            $variable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
        }

        if ($readOnly) {
            $flags = '1';
        } else {
            $flags = '0';
        }

        switch ($expression['right']['type']) {

            case 'array-access':
                $exprVariable = new Expression($expression['right']['left']);
                $exprVariable->setReadOnly(true);
                $exprVariable->setNoisy(false);

                $exprCompiledVariable = $exprVariable->compile($compilationContext);
                if ($exprCompiledVariable->getType() != 'variable') {
                    throw new CompilerException("Expression type: " . $exprCompiledVariable->getType() . " cannot be used as array", $expression['right']['left']);
                }

                $evalVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression['right']['left']);
                if ($evalVariable->getType() != 'variable' && $evalVariable->getType() != 'array') {
                    throw new CompilerException("Variable type: " . $variable->getType() . " cannot be used as array", $expression['right']['left']);
                }

                if ($evalVariable->getType() == 'variable') {
                    if ($evalVariable->hasDifferentDynamicType(array('undefined', 'array', 'null'))) {
                        $compilationContext->logger->warning('Possible attempt to use non array in fetch operator', 'non-valid-fetch', $expression['right']);
                    }
                }

                $expr = new Expression($expression['right']['right']);
                $expr->setReadOnly(true);
                $expr->setNoisy(false);
                $resolvedExpr = $expr->compile($compilationContext);

                switch ($resolvedExpr->getType()) {

                    case 'int':
                    case 'long':
                    case 'uint':
                        return new CompiledExpression('bool', 'zephir_array_isset_long_fetch(&' . $variable->getName() . ', ' . $evalVariable->getName() . ', ' . $resolvedExpr->getCode() . ', ' . $flags . ' TSRMLS_CC)', $expression);

                    case 'string':
                        return new CompiledExpression('bool', 'zephir_array_isset_string_fetch(&' . $variable->getName() . ', ' . $evalVariable->getName() . ', SS("' . $resolvedExpr->getCode() . '"), ' . $flags . ' TSRMLS_CC)', $expression);

                    case 'variable':
                        $indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $expression['right']['left']);
                        switch ($indexVariable->getType()) {

                            case 'int':
                            case 'long':
                            case 'uint':
                                return new CompiledExpression('bool', 'zephir_array_isset_long_fetch(&' . $variable->getName() . ', ' . $evalVariable->getName() . ', ' . $indexVariable->getName() . ', ' . $flags . ' TSRMLS_CC)', $expression);

                            case 'string':
                            case 'variable':
                                return new CompiledExpression('bool', 'zephir_array_isset_fetch(&' . $variable->getName() . ', ' . $evalVariable->getName() . ', ' . $indexVariable->getName() . ', ' . $flags . ' TSRMLS_CC)', $expression);

                            default:
                                throw new CompilerException('[' . $indexVariable->getType() . ']', $expression);
                        }
                        break;

                    default:
                        throw new CompilerException('[' . $expression['right']['right']['type'] . ']', $expression);
                }
                break;

            case 'property-access':

                $exprVariable = new Expression($expression['right']['left']);
                $exprVariable->setReadOnly(true);
                $exprVariable->setNoisy(false);

                $exprCompiledVariable = $exprVariable->compile($compilationContext);
                if ($exprCompiledVariable->getType() != 'variable') {
                    throw new CompilerException("Expression type: " . $exprCompiledVariable->getType() . " cannot be used as object", $expression['right']['left']);
                }

                $evalVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression['right']['left']);
                if ($evalVariable->getType() != 'variable') {
                    throw new CompilerException("Variable type: " . $variable->getType() . " cannot be used as object", $expression['right']['left']);
                }

                if ($evalVariable->hasDifferentDynamicType(array('undefined', 'object', 'null'))) {
                    $compilationContext->logger->warning('Possible attempt to use non object in fetch operator', 'non-valid-fetch', $expression['right']);
                }

                $property = $expression['right']['right']['value'];

                $compilationContext->headersManager->add('kernel/object');
                return new CompiledExpression('bool', 'zephir_fetch_property(&' . $variable->getName() . ', ' . $evalVariable->getName() . ', SL("' . $property . '"), PH_SILENT_CC)', $expression);

            case 'property-dynamic-access':

                $exprVariable = new Expression($expression['right']['left']);
                $exprVariable->setReadOnly(true);
                $exprVariable->setNoisy(false);

                $exprCompiledVariable = $exprVariable->compile($compilationContext);
                if ($exprCompiledVariable->getType() != 'variable') {
                    throw new CompilerException("Expression type: " . $exprCompiledVariable->getType() . " cannot be used as object", $expression['right']['left']);
                }

                $evalVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression['right']['left']);
                if ($evalVariable->getType() != 'variable') {
                    throw new CompilerException("Variable type: " . $evalVariable->getType() . " cannot be used as object", $expression['right']['left']);
                }

                if ($evalVariable->hasDifferentDynamicType(array('undefined', 'object', 'null'))) {
                    $compilationContext->logger->warning('Possible attempt to use non object in fetch operator', 'non-valid-fetch', $expression['right']);
                }

                $exprVariableProperty = new Expression($expression['right']['right']);
                $exprVariableProperty->setReadOnly(true);

                $exprCompiledVariableProperty = $exprVariableProperty->compile($compilationContext);
                if ($exprCompiledVariableProperty->getType() != 'variable') {
                    throw new CompilerException("Expression type: " . $exprCompiledVariableProperty->getType() . " cannot be used in property-dynamic-acces", $expression['right']['right']);
                }

                $evalVariableProperty = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariableProperty->getCode(), $compilationContext, $expression['right']['right']);
                if ($evalVariableProperty->getType() != 'variable' && $evalVariableProperty->getType() != 'string') {
                    throw new CompilerException("Variable type: " . $evalVariableProperty->getType() . " cannot be used in property-dynamic-access", $expression['right']['right']);
                }

                $compilationContext->headersManager->add('kernel/object');

                return new CompiledExpression('bool', 'zephir_fetch_property_zval(&' . $variable->getName() . ', ' . $evalVariable->getName() . ', ' . $evalVariableProperty->getName() . ', PH_SILENT_CC)', $expression);

            default:
                throw new CompilerException('Cannot use this expression for "fetch" operators', $expression);
        }
    }
}
