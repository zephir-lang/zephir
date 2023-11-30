<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Operators\Other;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;

/**
 * Checks if an array offset or a property is defined on a polymorphic variable
 */
class IssetOperator extends AbstractOperator
{
    /**
     * Compiles an 'isset' operator.
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $left = 'list' === $expression['left']['type'] ? $expression['left']['left'] : $expression['left'];

        switch ($left['type']) {
            case 'array-access':
                $compilationContext->headersManager->add('kernel/array');

                $exprVariable = new Expression($left['left']);
                $exprVariable->setReadOnly(true);
                $exprVariable->setNoisy(false);

                $exprCompiledVariable = $exprVariable->compile($compilationContext);
                if (!in_array($exprCompiledVariable->getType(), ['variable', 'array'], true)) {
                    throw new CompilerException(
                        'Expression type: ' . $exprCompiledVariable->getType() . ' cannot be used as array',
                        $left['left']
                    );
                }

                $variable = $compilationContext->symbolTable->getVariableForRead(
                    $exprCompiledVariable->getCode(),
                    $compilationContext,
                    $left['left']
                );
                switch ($variable->getType()) {
                    case 'array':
                    case 'variable':
                        break;

                    default:
                        throw new CompilerException(
                            'Variable type: ' . $variable->getType() . ' cannot be used as array',
                            $left['left']
                        );
                }

                if ('variable' === $variable->getType()) {
                    if ($variable->hasDifferentDynamicType(['undefined', 'array', 'null'])) {
                        $compilationContext->logger->warning(
                            'Possible attempt to use non array in isset operator',
                            ['non-valid-isset', $expression]
                        );
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
                        return $compilationContext->backend->arrayIsset(
                            $variable,
                            $resolvedExpr,
                            $left['right'],
                            $compilationContext
                        );

                    case 'variable':
                    case 'mixed':
                        $indexVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolvedExpr->getCode(),
                            $compilationContext,
                            $left['right']
                        );

                        return $compilationContext->backend->arrayIsset(
                            $variable,
                            $indexVariable,
                            $left['right'],
                            $compilationContext
                        );


                    default:
                        throw new CompilerException('[' . $left['right']['type'] . ']', $expression);
                }


            case 'property-access':
            case 'property-dynamic-access':
                $compilationContext->headersManager->add('kernel/object');

                $exprVariable = new Expression($left['left']);
                $exprVariable->setReadOnly(true);

                $exprCompiledVariable = $exprVariable->compile($compilationContext);
                if ('variable' !== $exprCompiledVariable->getType()) {
                    throw new CompilerException(
                        'Expression type: ' . $exprCompiledVariable->getType() . ' cannot be used as object',
                        $left['left']
                    );
                }

                $variable = $compilationContext->symbolTable->getVariableForRead(
                    $exprCompiledVariable->getCode(),
                    $compilationContext,
                    $left['left']
                );
                if ('variable' !== $variable->getType()) {
                    throw new CompilerException(
                        'Variable type: ' . $variable->getType() . ' cannot be used as object',
                        $left['left']
                    );
                }

                if ($variable->hasDifferentDynamicType(['undefined', 'object', 'null'])) {
                    $compilationContext->logger->warning(
                        'Possible attempt to use non object in isset operator',
                        ['non-valid-isset', $expression]
                    );
                }
                $variableCode = $compilationContext->backend->getVariableCode($variable);

                if ('property-access' === $left['type']) {
                    return $compilationContext->backend->propertyIsset($variable, $left['right']['value']);
                }

                $expr = new Expression($left['right']);
                $expr->setReadOnly(true);
                $expr->setNoisy(false);
                $resolvedExpr = $expr->compile($compilationContext);

                switch ($resolvedExpr->getType()) {
                    case 'variable':
                        $indexVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolvedExpr->getCode(),
                            $compilationContext,
                            $left['right']
                        );
                        switch ($indexVariable->getType()) {
                            case 'variable':
                            case 'mixed':
                            case 'string':
                                $indexVariableCode = $compilationContext->backend->getVariableCode($indexVariable);

                                return new CompiledExpression(
                                    'bool',
                                    'zephir_isset_property_zval(' . $variableCode . ', ' . $indexVariableCode . ')',
                                    $left['right']
                                );

                            default:
                                throw new CompilerException('[' . $indexVariable->getType() . ']', $expression);
                        }


                    default:
                        throw new CompilerException('[' . $expression['left']['right']['type'] . ']', $expression);
                }


            case 'property-string-access':
            case 'static-property-access':
                return new CompiledExpression('bool', '(0 == 0)', $left);

            default:
                throw new CompilerException('This expression is not valid for "isset" operator', $expression);
        }
    }
}
