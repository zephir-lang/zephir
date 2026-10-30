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
use Zephir\Variable\Variable;

/**
 * Checks if an array offset or a property is defined on a polymorphic variable
 */
class IssetOperator extends AbstractOperator
{
    /**
     * Compiles an 'isset' operator.
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

                    /**
                     * `isset(s[i])` on a declared string. The kernel isset
                     * helpers dispatch on the container's runtime type, so the
                     * ordinary emission below already does the right thing for
                     * a string: PHP's silent, negative-aware offset test.
                     */
                    case 'string':
                    case 'istring':
                        $compilationContext->headersManager->add('kernel/string');
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
                $resolvedExpr = $compilationContext->backend->boxArrayKey($resolvedExpr, $compilationContext);

                switch ($resolvedExpr->getType()) {
                    case 'int':
                    case 'long':
                    case 'string':
                        // isset throws for an illegal key or from offsetExists()
                        $isset = $compilationContext->backend->arrayIsset($variable, $resolvedExpr, $left['right']);

                        return new CompiledExpression(
                            'bool',
                            $compilationContext->emitCheckedBool($isset->getCode()),
                            $left['right']
                        );

                    case 'variable':
                    case 'mixed':
                        $indexVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolvedExpr->getCode(),
                            $compilationContext,
                            $left['right']
                        );

                        // isset throws for an illegal key or from offsetExists()
                        $isset = $compilationContext->backend->arrayIsset($variable, $indexVariable, $left['right']);

                        return new CompiledExpression(
                            'bool',
                            $compilationContext->emitCheckedBool($isset->getCode()),
                            $left['right']
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
                    return $compilationContext->backend->propertyIsset(
                        $variable,
                        $left['right']['value'],
                        $compilationContext,
                    );
                }

                $expr = new Expression($left['right']);
                $expr->setReadOnly(true);
                $expr->setNoisy(false);
                $resolvedExpr = $expr->compile($compilationContext);
                $resolvedExpr = $compilationContext->backend->boxArrayKey($resolvedExpr, $compilationContext);

                switch ($resolvedExpr->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $indexVariable = $this->boxIntegerKey($resolvedExpr->getCode(), $compilationContext);
                        break;

                    case 'variable':
                        $indexVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolvedExpr->getCode(),
                            $compilationContext,
                            $left['right']
                        );
                        if (in_array($indexVariable->getType(), ['int', 'uint', 'long', 'ulong'], true)) {
                            $indexVariable = $this->boxIntegerKey($indexVariable, $compilationContext);
                        }
                        break;

                    default:
                        throw new CompilerException('[' . $expression['left']['right']['type'] . ']', $expression);
                }

                if (!in_array($indexVariable->getType(), ['variable', 'mixed', 'string'], true)) {
                    throw new CompilerException('[' . $indexVariable->getType() . ']', $expression);
                }

                $indexVariableCode = $compilationContext->backend->getVariableCode($indexVariable);

                /*
                 * PHP isset() semantics, see https://github.com/zephir-lang/zephir/issues/2385;
                 * it throws for an illegal key or from offsetExists().
                 */
                return new CompiledExpression(
                    'bool',
                    $compilationContext->emitCheckedBool(
                        'zephir_isset_property_value_zval(' . $variableCode . ', ' . $indexVariableCode . ')'
                    ),
                    $left['right']
                );


            case 'property-string-access':
            case 'static-property-access':
                return new CompiledExpression('bool', '(0 == 0)', $left);

            default:
                throw new CompilerException('This expression is not valid for "isset" operator', $expression);
        }
    }

    /**
     * An integer key as a temp zval, for the zval-keyed property helpers.
     */
    private function boxIntegerKey($value, CompilationContext $compilationContext): Variable
    {
        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
        $compilationContext->backend->assignLong($tempVariable, $value, $compilationContext);

        return $tempVariable;
    }
}
