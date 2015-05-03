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
use Zephir\Optimizers\EvalExpression;

/**
 * IssetOperator
 *
 * Checks if a array offset or a property is defined on a polymorphic variable
 */
class IssetOperator extends BaseOperator
{
    /**
     * Compiles an 'isset' operator
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        if ($expression['left']['type'] == 'list') {
            $left = $expression['left']['left'];
        } else {
            $left = $expression['left'];
        }

        switch ($left['type']) {

            case 'array-access':

                $compilationContext->headersManager->add('kernel/array');

                $exprVariable = new Expression($left['left']);
                $exprVariable->setReadOnly(true);
                $exprVariable->setNoisy(false);

                $exprCompiledVariable = $exprVariable->compile($compilationContext);
                if ($exprCompiledVariable->getType() != 'variable' && $exprCompiledVariable->getType() != 'array') {
                    throw new CompilerException("Expression type: " . $exprCompiledVariable->getType() . " cannot be used as array", $left['left']);
                }

                $variable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $left['left']);
                switch ($variable->getType()) {

                    case 'array':
                    case 'variable':
                        break;

                    default:
                        throw new CompilerException("Variable type: " . $variable->getType() . " cannot be used as array", $left['left']);
                        break;
                }

                if ($variable->getType() == 'variable') {
                    if ($variable->hasDifferentDynamicType(array('undefined', 'array', 'null'))) {
                        $compilationContext->logger->warning('Possible attempt to use non array in isset operator', 'non-valid-isset', $expression);
                    }
                }

                $expr = new Expression($left['right']);
                $expr->setReadOnly(true);
                $expr->setNoisy(false);
                $resolvedExpr = $expr->compile($compilationContext);

                switch ($resolvedExpr->getType()) {

                    case 'int':
                    case 'long':
                        return new CompiledExpression('bool', 'zephir_array_isset_long(' . $variable->getName() . ', ' . $resolvedExpr->getCode() . ')', $left['right']);

                    case 'string':
                        return new CompiledExpression('bool', 'zephir_array_isset_string(' . $variable->getName() . ', SS("' . $resolvedExpr->getCode() . '"))', $left['right']);

                    case 'variable':
                        $indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $left['right']);
                        $indexVariableName = ($indexVariable->isLocalOnly() ? '&' : '') . $indexVariable->getName();

                        switch ($indexVariable->getType()) {

                            case 'int':
                            case 'long':
                                return new CompiledExpression('bool', 'zephir_array_isset_long(' . $variable->getName() . ', ' . $indexVariableName . ')', $left['right']);

                            case 'variable':
                            case 'string':
                                return new CompiledExpression('bool', 'zephir_array_isset(' . $variable->getName() . ', ' . $indexVariableName . ')', $left['right']);

                            default:
                                throw new CompilerException('[' . $indexVariable->getType() . ']', $expression);
                        }
                        break;

                    default:
                        throw new CompilerException('[' . $left['right']['type'] . ']', $expression);
                }
                break;

            case 'property-access':
            case 'property-dynamic-access':

                $compilationContext->headersManager->add('kernel/object');

                $exprVariable = new Expression($left['left']);
                $exprVariable->setReadOnly(true);

                $exprCompiledVariable = $exprVariable->compile($compilationContext);
                if ($exprCompiledVariable->getType() != 'variable') {
                    throw new CompilerException("Expression type: " . $exprCompiledVariable->getType() . " cannot be used as object", $left['left']);
                }

                $variable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $left['left']);
                if ($variable->getType() != 'variable') {
                    throw new CompilerException("Variable type: " . $variable->getType() . " cannot be used as object", $left['left']);
                }

                if ($variable->hasDifferentDynamicType(array('undefined', 'object', 'null'))) {
                    $compilationContext->logger->warning('Possible attempt to use non object in isset operator', 'non-valid-isset', $expression);
                }

                if ($left['type'] == 'property-access') {
                    return new CompiledExpression('bool', 'zephir_isset_property(' . $variable->getName() . ', SS("' . $left['right']['value'] . '") TSRMLS_CC)', $left);
                }

                $expr = new Expression($left['right']);
                $expr->setReadOnly(true);
                $expr->setNoisy(false);
                $resolvedExpr = $expr->compile($compilationContext);

                switch ($resolvedExpr->getType()) {

                    case 'variable':
                        $indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $left['right']);
                        switch ($indexVariable->getType()) {

                            case 'variable':
                            case 'string':
                                return new CompiledExpression('bool', 'zephir_isset_property_zval(' . $variable->getName() . ', ' . $indexVariable->getName() . ' TSRMLS_CC)', $left['right']);

                            default:
                                throw new CompilerException('[' . $indexVariable->getType() . ']', $expression);
                        }
                        break;

                    default:
                        throw new CompilerException('[' . $expression['left']['right']['type'] . ']', $expression);
                }
                break;

            case 'property-string-access':
                return new CompiledExpression('bool', '(0 == 0)', $left);

            case 'static-property-access':
                return new CompiledExpression('bool', '(0 == 0)', $left);

            default:
                throw new CompilerException('This expression is not valid for "isset" operator', $expression);
        }
    }
}
