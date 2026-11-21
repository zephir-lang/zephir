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

use function in_array;
use function is_array;

/**
 * Fetch is a special operator that checks if an expression 'isset' and then obtain the value
 * without calculating the hash key twice
 */
class FetchOperator extends AbstractOperator
{
    /**
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $compilationContext->headersManager->add('kernel/array');

        $variable = $compilationContext->symbolTable->getVariableForWrite(
            $expression['left']['value'],
            $compilationContext,
            $expression['left']
        );
        if ('variable' != $variable->getType()) {
            throw CompilerException::cannotUseVariableTypeAs(
                $variable,
                'in "fetch" operator',
                $expression
            );
        }

        /**
         * `fetch a, a[k]` and `fetch k, a[k]` read the variable they write.
         * Observing it first would release the container or the key before
         * the fetch looks at it, so the value is fetched into a temporary and
         * copied in afterwards, as `let` does.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2832
         */
        $readsTarget = 'return_value' !== $variable->getName()
            && $this->readsVariable($expression['right'], $variable->getName());

        $readOnly = false;
        $target   = $variable;

        if ($readsTarget) {
            $target = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext);
            $variable->setIsInitialized(true, $compilationContext);
            $variable->setDynamicTypes('undefined');
            $variable->setPossibleValue(new CompiledExpression('undefined', '', $expression), $compilationContext);
        } elseif ('return_value' !== $variable->getName()) {
            /**
             * return_value must not be observed
             *
             * TODO: use a read detector here
             */
            $line = max(
                $compilationContext->symbolTable->getLastCallLine(),
                $compilationContext->symbolTable->getLastUnsetLine()
            );

            /**
             * A read-only target borrows: it is not observed and the memory
             * frame never releases it, so only a container that owns the value
             * can back it. An ArrayAccess container owns nothing once
             * offsetGet() has returned. Unlike `let x = c["k"]`, this
             * shortcut is not worth proving a container for: every `fetch`
             * site in the stub takes an untyped container, and the saving is
             * one refcount pair, so NativeArrayPass is deliberately not
             * consulted here and the shortcut is simply dropped.
             *
             * `zephir_fetch_property()` copies, so property access is
             * unaffected.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2682
             */
            $canBorrow = 'array-access' !== $expression['right']['type'];

            if ($canBorrow && (false === $line || ($line > 0 && $line < $expression['line']))) {
                $numberMutations = $compilationContext->symbolTable->getExpectedMutations($variable->getName());
                if (1 == $numberMutations) {
                    if (1 == $variable->getNumberMutations()) {
                        $variable->setIsInitialized(true, $compilationContext);
                        $variable->setMemoryTracked(false);
                        $variable->setDynamicTypes('undefined');
                        $readOnly = true;
                    }
                }
            }

            if (!$readOnly || 'array-access' != $expression['right']['type']) {
                $variable->setIsInitialized(true, $compilationContext);
                $variable->observeVariant($compilationContext);
                $variable->setDynamicTypes('undefined');
                $variable->setPossibleValue(new CompiledExpression('undefined', '', $expression), $compilationContext);
            }
        } else {
            $target = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext);
        }

        $flags = $readOnly ? '1' : '0';

        switch ($expression['right']['type']) {
            case 'array-access':
                $fetched = $this->arrayFetch($expression, $target, $flags, $compilationContext);
                break;

            case 'property-access':
            case 'property-string-access':
            case 'property-dynamic-access':
                $fetched = $this->propertyFetch($expression['right'], $target, $compilationContext);
                break;

            default:
                throw new CompilerException(
                    'Cannot use this expression for "fetch" operators: ' . $expression['right']['type'],
                    $expression
                );
        }

        // fetch throws for an illegal key, from offsetExists()/offsetGet(), from __isset()/__get() or for a name
        $found = $compilationContext->emitCheckedBool($fetched);

        if ($readsTarget) {
            $compilationContext->backend->copyOnWrite($variable, $target, $compilationContext);
        }

        return new CompiledExpression('bool', $found, $expression);
    }

    /**
     * `fetch value, a[key]`: the kernel call that fills `target`.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function arrayFetch(
        array $expression,
        Variable $target,
        string $flags,
        CompilationContext $compilationContext
    ): string {
        $exprVariable = new Expression($expression['right']['left']);
        $exprVariable->setReadOnly(true);
        $exprVariable->setNoisy(false);

        $exprCompiledVariable = $exprVariable->compile($compilationContext);
        if ('variable' != $exprCompiledVariable->getType()) {
            throw new CompilerException(
                'Expression type: ' . $exprCompiledVariable->getType() . ' cannot be used as array',
                $expression['right']['left']
            );
        }

        $evalVariable = $compilationContext->symbolTable->getVariableForRead(
            $exprCompiledVariable->getCode(),
            $compilationContext,
            $expression['right']['left']
        );
        if ('variable' != $evalVariable->getType() && 'array' != $evalVariable->getType()) {
            throw new CompilerException(
                'Variable type: ' . $target->getType() . ' cannot be used as array',
                $expression['right']['left']
            );
        }

        if ('variable' == $evalVariable->getType()) {
            if ($evalVariable->hasDifferentDynamicType(['undefined', 'array', 'null'])) {
                $compilationContext->logger->warning(
                    'Possible attempt to use non array in fetch operator',
                    ['non-valid-fetch', $expression['right']]
                );
            }
        }

        // The key is an ordinary read, as in PHP: only the container is fetched silently (#2832)
        $expr = new Expression($expression['right']['right']);
        $expr->setReadOnly(true);
        $resolvedExpr = $expr->compile($compilationContext);
        $resolvedExpr = $compilationContext->backend->boxArrayKey($resolvedExpr, $compilationContext);

        return $compilationContext->backend->arrayIssetFetch(
            $target,
            $evalVariable,
            $resolvedExpr,
            $flags,
            $expression,
            $compilationContext
        )->getCode();
    }

    /**
     * `fetch value, o->x`, `o->{"x"}` or `o->{name}`: the kernel call that
     * fills `target`. The name takes every form `isset` takes.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function propertyFetch(array $right, Variable $target, CompilationContext $compilationContext): string
    {
        $exprVariable = new Expression($right['left']);
        $exprVariable->setReadOnly(true);
        $exprVariable->setNoisy(false);

        $exprCompiledVariable = $exprVariable->compile($compilationContext);
        if ('variable' != $exprCompiledVariable->getType()) {
            throw new CompilerException(
                'Expression type: ' . $exprCompiledVariable->getType() . ' cannot be used as object',
                $right['left']
            );
        }

        $evalVariable = $compilationContext->symbolTable->getVariableForRead(
            $exprCompiledVariable->getCode(),
            $compilationContext,
            $right['left']
        );
        if ('variable' != $evalVariable->getType()) {
            throw new CompilerException(
                'Variable type: ' . $evalVariable->getType() . ' cannot be used as object',
                $right['left']
            );
        }

        if ($evalVariable->hasDifferentDynamicType(['undefined', 'object', 'null'])) {
            $compilationContext->logger->warning(
                'Possible attempt to use non object in fetch operator',
                ['non-valid-fetch', $right]
            );
        }

        $compilationContext->headersManager->add('kernel/object');

        $symbol     = $compilationContext->backend->getVariableCode($target);
        $evalSymbol = $compilationContext->backend->getVariableCode($evalVariable);

        /* A name written as a string, `o->{"x"}`, is as known as `o->x` */
        if ('property-dynamic-access' !== $right['type']) {
            return 'zephir_fetch_property(' . $symbol . ', ' . $evalSymbol
                . ', SL("' . $right['right']['value'] . '"), PH_SILENT_CC)';
        }

        $nameVariable = (new IssetOperator())->propertyNameVariable($right, $compilationContext);

        return 'zephir_fetch_property_zval(' . $symbol . ', ' . $evalSymbol . ', '
            . $compilationContext->backend->getVariableCode($nameVariable) . ', PH_SILENT_CC)';
    }

    /**
     * Whether an expression reads the variable `name`. The name in `o->x` is
     * not a variable.
     */
    private function readsVariable(array $node, string $name): bool
    {
        if ('variable' === ($node['type'] ?? null)) {
            return $name === ($node['value'] ?? null);
        }

        if (in_array($node['type'] ?? null, ['property-access', 'property-string-access'], true)) {
            return $this->readsVariable($node['left'], $name);
        }

        foreach ($node as $child) {
            if (is_array($child) && $this->readsVariable($child, $name)) {
                return true;
            }
        }

        return false;
    }
}
