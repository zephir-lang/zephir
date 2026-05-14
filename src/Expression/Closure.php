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

namespace Zephir\Expression;

use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Class\Method\Parameters;
use Zephir\Class\Property;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\CompilerFileAnonymous;
use Zephir\Exception;
use Zephir\StatementsBlock;
use Zephir\Variable\Variable;

use function in_array;
use function is_array;

/**
 * Creates an anonymous function within the extension simulating a closure
 */
class Closure
{
    protected bool $expecting = true;

    protected ?Variable $expectingVariable = null;
    /**
     * Unique closure ID.
     */
    protected static $id = 0;

    protected bool $readOnly = false;

    /**
     * Recursively checks if an AST node references `this`.
     *
     * Two AST shapes need to be detected:
     *   1. Read of `this` — `{type: variable, value: this}` (e.g. inside
     *      `return this->prop` or `this->method()` expressions).
     *   2. Write to `this->prop` — let-statement assignments encode the
     *      target object as a plain string field on the assignment node:
     *      `{assign-type: object-property*, variable: "this", property: ...}`.
     *      Without recognising this, `let this->prop = …` inside a closure
     *      slips past the binding check, the enclosing class isn't carried
     *      over and ObjectProperty::assign reports the property missing on
     *      the synthetic closure class. (see #1873.)
     */
    protected static function astReferencesThis(mixed $node): bool
    {
        if (!is_array($node)) {
            return false;
        }

        // Read: `{type: variable, value: this}` (e.g. inside expressions).
        if (isset($node['type']) && $node['type'] === 'variable' && isset($node['value']) && $node['value'] === 'this') {
            return true;
        }

        // Write: let-statement assignment target. `assign-type` ∈
        // {object-property, object-property-append, object-property-array-index, ...}
        // and the `variable` field is the bare string `"this"`.
        if (
            isset($node['assign-type'], $node['variable'])
            && $node['variable'] === 'this'
            && \is_string($node['assign-type'])
            && \str_starts_with($node['assign-type'], 'object-property')
        ) {
            return true;
        }

        // Recurse into all array children
        foreach ($node as $child) {
            if (is_array($child) && self::astReferencesThis($child)) {
                return true;
            }
        }

        return false;
    }

    /**
     * Creates a closure.
     *
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $classDefinition = new Definition(
            $compilationContext->config->get('namespace'),
            self::$id . '__closure'
        );

        $classDefinition->setIsFinal(true);

        $compilerFile = new CompilerFileAnonymous($classDefinition, $compilationContext->config, $compilationContext);
        $compilerFile->setLogger($compilationContext->logger);

        $compilationContext->compiler->addClassDefinition($compilerFile, $classDefinition);

        $parameters = null;
        if (isset($expression['left'])) {
            $parameters = new Parameters($expression['left']);
        }

        $block = $expression['right'] ?? [];

        $staticVariables = [];
        if (isset($expression['use']) && is_array($expression['use'])) {
            foreach ($expression['use'] as $parameter) {
                $staticVariables[$parameter['name']] = $compilationContext->symbolTable->getVariable(
                    $parameter['name']
                );
            }
        }

        foreach ($staticVariables as $var) {
            $classDefinition->addProperty(
                new Property(
                    $classDefinition,
                    ['public', 'static'],
                    $var->getName(),
                    null,
                    null,
                    null
                )
            );
        }

        $classMethod = new Method(
            $classDefinition,
            ['public', 'final'],
            '__invoke',
            $parameters,
            new StatementsBlock($block),
            null,
            null,
            $expression,
            $staticVariables
        );

        /**
         * Detect if the closure body references `this`.
         * If so, we need to bind the enclosing object's `this_ptr` to the closure
         * and set the enclosing class definition for compile-time resolution.
         */
        $bindThis = self::astReferencesThis($block);
        if ($bindThis) {
            $classDefinition->setEnclosingClassDefinition($compilationContext->classDefinition);

            // Ensure this_ptr is declared and not stripped in the enclosing method
            if ($compilationContext->symbolTable->hasVariable('this')) {
                $compilationContext->symbolTable->getVariable('this')->setUsed(true);
            }
        }

        $symbolVariable = $this->generateClosure(
            $classDefinition,
            $classMethod,
            $block,
            $compilationContext,
            $expression,
            $bindThis
        );
        $compilationContext->headersManager->add('kernel/object');

        foreach ($staticVariables as $var) {
            if (in_array($var->getType(), ['variable', 'array'])) {
                $compilationContext->backend->updateStaticProperty(
                    $classDefinition->getClassEntry(),
                    $var->getName(),
                    $var,
                    $compilationContext
                );
                continue;
            }

            $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                'variable',
                $compilationContext,
                true
            );

            switch ($var->getType()) {
                case 'int':
                case 'uint':
                case 'long':
                case 'ulong':
                case 'char':
                case 'uchar':
                    $compilationContext->backend->assignLong($tempVariable, $var, $compilationContext);
                    break;
                case 'double':
                    $compilationContext->backend->assignDouble($tempVariable, $var, $compilationContext);
                    break;
                case 'bool':
                    $compilationContext->backend->assignBool($tempVariable, $var, $compilationContext);
                    break;
                case 'string':
                    $compilationContext->backend->assignString($tempVariable, $var, $compilationContext);
                    break;
                default:
                    break;
            }

            $compilationContext->backend->updateStaticProperty(
                $classDefinition->getClassEntry(),
                $var->getName(),
                $tempVariable,
                $compilationContext
            );
        }

        ++self::$id;

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     */
    public function setExpectReturn(bool $expecting, ?Variable $expectingVariable = null): void
    {
        $this->expecting         = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     */
    public function setReadOnly(bool $readOnly): void
    {
        $this->readOnly = $readOnly;
    }

    /**
     * @param Definition         $classDefinition
     * @param Method             $classMethod
     * @param mixed              $block
     * @param CompilationContext $compilationContext
     * @param array              $expression
     *
     * @return Variable|null
     */
    protected function generateClosure(
        Definition $classDefinition,
        Method $classMethod,
        mixed $block,
        CompilationContext $compilationContext,
        array $expression,
        bool $bindThis = false
    ): ?Variable {
        $classDefinition->addMethod($classMethod, $block);

        $compilationContext->headersManager->add('kernel/object');

        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext,
                    $expression
                );
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'variable',
                $compilationContext,
                $expression
            );
        }

        $symbolVariable->initVariant($compilationContext);
        $compilationContext->backend->createClosure($symbolVariable, $classDefinition, $compilationContext, $bindThis);

        return $symbolVariable;
    }
}
