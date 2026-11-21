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
use Zephir\Class\Visibility;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Expression\StaticPropertyAccess;
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

                // The key is an ordinary read, as in PHP: only the container is fetched silently (#2832)
                $expr = new Expression($left['right']);
                $expr->setReadOnly(true);
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
            case 'property-string-access':
            case 'property-dynamic-access':
                return $this->propertyIssetOrEmpty($left, $compilationContext, false);

            case 'static-property-access':
                return $this->staticPropertyIsset($left, $compilationContext);

            default:
                throw new CompilerException('This expression is not valid for "isset" operator', $expression);
        }
    }

    /**
     * `isset o->x` and, with `$empty`, `empty o->x`, as PHP's
     * ZEND_ISSET_ISEMPTY_PROP_OBJ answers them: the object is fetched as
     * silently as `FETCH_OBJ_IS` does, and its has_property handler decides,
     * which asks `__isset()` first. `__isset()` and `__get()` may throw, and
     * the method stops there.
     *
     * @throws Exception
     * @throws ReflectionException
     *
     * @see https://github.com/zephir-lang/zephir/issues/2831
     */
    public function propertyIssetOrEmpty(
        array $left,
        CompilationContext $compilationContext,
        bool $empty
    ): CompiledExpression {
        $compilationContext->headersManager->add('kernel/object');

        $exprVariable = new Expression($left['left']);
        $exprVariable->setReadOnly(true);
        $exprVariable->setNoisy(false);

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

        if (!$empty && $variable->hasDifferentDynamicType(['undefined', 'object', 'null'])) {
            $compilationContext->logger->warning(
                'Possible attempt to use non object in isset operator',
                ['non-valid-isset', $left]
            );
        }
        $variableCode = $compilationContext->backend->getVariableCode($variable);

        /* A name written as a string, `this->{"x"}`, is as known as `this->x` */
        if ('property-dynamic-access' !== $left['type']) {
            $isset = $compilationContext->backend->propertyIsset(
                $variable,
                $left['right']['value'],
                $compilationContext,
                $empty
            );

            return new CompiledExpression('bool', $compilationContext->emitCheckedBool($isset->getCode()), $left);
        }

        $indexVariable = $this->propertyNameVariable($left, $compilationContext);

        $function = $empty ? 'zephir_isempty_property_zval' : 'zephir_isset_property_value_zval';

        return new CompiledExpression(
            'bool',
            $compilationContext->emitCheckedBool(
                $function . '(' . $variableCode . ', '
                . $compilationContext->backend->getVariableCode($indexVariable) . ')'
            ),
            $left['right']
        );
    }

    /**
     * The variable holding the name of `o->{name}`, an integer boxed, for the
     * kernel helpers that turn it into a string as PHP does. `fetch` names its
     * property the same way.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function propertyNameVariable(array $left, CompilationContext $compilationContext): Variable
    {
        $expr = new Expression($left['right']);
        $expr->setReadOnly(true);
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
                throw new CompilerException('[' . $left['right']['type'] . ']', $left);
        }

        if (!in_array($indexVariable->getType(), ['variable', 'mixed', 'string'], true)) {
            throw new CompilerException('[' . $indexVariable->getType() . ']', $left);
        }

        return $indexVariable;
    }

    /**
     * `isset X::y` as PHP's ZEND_ISSET_ISEMPTY_STATIC_PROP answers it: false
     * for a property that is not declared, not static or not visible from
     * here, all of which are known when compiling, and otherwise whether the
     * property holds something other than null.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2805
     */
    private function staticPropertyIsset(array $left, CompilationContext $compilationContext): CompiledExpression
    {
        $property        = $left['right']['value'];
        $classDefinition = (new StaticPropertyAccess())->classDefinitionOf($left, $compilationContext);

        if (null === $classDefinition || !$classDefinition->hasProperty($property)) {
            return new CompiledExpression('bool', '0', $left);
        }

        $propertyDefinition = $classDefinition->getProperty($property);
        if (!$propertyDefinition->isStatic() || !Visibility::canAccessProperty($propertyDefinition, $compilationContext)) {
            return new CompiledExpression('bool', '0', $left);
        }

        return $compilationContext->backend->staticPropertyIsset($classDefinition, $property, $compilationContext);
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
