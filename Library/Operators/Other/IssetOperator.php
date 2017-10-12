<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Operators\Other;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\Compiler\CompilerException;
use Zephir\CompiledExpression;

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
                    case 'string':
                        return $compilationContext->backend->arrayIsset($variable, $resolvedExpr, $left['right'], $compilationContext);

                    case 'variable':
                        $indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $left['right']);

                        return $compilationContext->backend->arrayIsset($variable, $indexVariable, $left['right'], $compilationContext);
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
                $variableCode = $compilationContext->backend->getVariableCode($variable);

                if ($left['type'] == 'property-access') {
                    return $compilationContext->backend->propertyIsset($variable, $left['right']['value'], $compilationContext);
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
                                $indexVariableCode = $compilationContext->backend->getVariableCode($indexVariable);
                                return new CompiledExpression('bool', 'zephir_isset_property_zval(' . $variableCode . ', ' . $indexVariableCode . ' TSRMLS_CC)', $left['right']);

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
