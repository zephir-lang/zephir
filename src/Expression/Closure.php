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
use Zephir\Exception\CompilerException;
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
                $captured = $compilationContext->symbolTable->getVariable(
                    $parameter['name'],
                    $compilationContext
                );

                if (!$captured instanceof Variable) {
                    throw new CompilerException(
                        "Cannot capture variable '" . $parameter['name'] . "' because it wasn't declared",
                        $parameter
                    );
                }

                /**
                 * A capture is a read of the enclosing variable, so it has to be
                 * counted as one. Method::compile() gates both the
                 * `unused-variable` warning and the variable's C declaration on
                 * the very same use count, so a variable whose only consumer is
                 * this clause used to be reported as unused *and* left
                 * undeclared - the generated C then referenced it anyway and
                 * failed to build.
                 *
                 * @see https://github.com/zephir-lang/zephir/issues/2029
                 */
                $captured->increaseUses();
                $captured->setUsed(true, $parameter);

                /**
                 * The by-reference flag belongs to this one clause, not to the
                 * variable: the same local can be captured by value by one
                 * closure and by reference by another.
                 *
                 * @see https://github.com/zephir-lang/zephir/issues/2652
                 */
                $capture = clone $captured;
                $capture->setIsClosureReference(!empty($parameter['reference']));

                $staticVariables[$parameter['name']] = $capture;
            }
        }

        /**
         * Detect if the closure body references `this`.
         * If so, we need to bind the enclosing object to the closure and set
         * the enclosing class definition for compile-time resolution.
         */
        $bindThis = self::astReferencesThis($block);
        if ($bindThis) {
            $classDefinition->setEnclosingClassDefinition($compilationContext->classDefinition);

            // Ensure this_ptr is declared and not stripped in the enclosing method
            if ($compilationContext->symbolTable->hasVariable('this')) {
                $compilationContext->symbolTable->getVariable('this')->setUsed(true);
            }
        }

        /**
         * A capturing closure binds a per-creation carrier object as its
         * `$this`, because that is the only per-instance slot the engine gives
         * an internal-function closure. The enclosing object therefore has to
         * ride along on the carrier as one more capture, under a name no
         * Zephir identifier can spell.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2652
         */
        if ([] !== $staticVariables && $bindThis) {
            $boundThis = new Variable(
                'variable',
                Method::CLOSURE_BOUND_THIS,
                $compilationContext->branchManager->getCurrentBranch()
            );
            $boundThis->setIsInitialized(true, $compilationContext);
            $boundThis->setDynamicTypes('object');
            $boundThis->increaseUses();
            $boundThis->setUsed(true);

            $staticVariables[Method::CLOSURE_BOUND_THIS] = $boundThis;
        }

        /**
         * Captures live in instance properties of the carrier, one carrier per
         * closure creation. They used to be `public static` properties, which
         * belong to the class and so were shared by every closure built from
         * this same source line.
         */
        foreach ($staticVariables as $var) {
            $classDefinition->addProperty(
                new Property(
                    $classDefinition,
                    ['public'],
                    $var->getName(),
                    null,
                    null,
                    null
                )
            );
        }

        /**
         * A closure's return type belongs to the synthetic `__invoke`, so it
         * flows through the ordinary method path: arg-info, docblock and engine
         * enforcement all come for free.
         *
         * @see https://github.com/zephir-lang/zephir/issues/1841
         */
        $classMethod = new Method(
            $classDefinition,
            ['public', 'final'],
            '__invoke',
            $parameters,
            new StatementsBlock($block),
            null,
            $expression['return-type'] ?? null,
            $expression,
            $staticVariables
        );

        $compilationContext->headersManager->add('kernel/object');

        /**
         * The carrier and its captures must exist before the closure is
         * created, so that the closure never observes a half-filled carrier.
         *
         * Known limitation: `$closure->bindTo($other)` replaces the carrier,
         * so the captures read back as null where PHP would keep them. The
         * bound `$this` is the only per-instance slot the engine gives an
         * internal-function closure, so there is nowhere else to put them.
         */
        $carrier = null;
        if ([] !== $staticVariables) {
            $carrier = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
            $compilationContext->backend->initObject(
                $carrier,
                $classDefinition->getClassEntry(),
                $compilationContext
            );

            foreach ($staticVariables as $var) {
                $name = $var->getName();

                if (Method::CLOSURE_BOUND_THIS === $name) {
                    $compilationContext->backend->updateClosureCapture(
                        $carrier,
                        $name,
                        $compilationContext->symbolTable->getVariable('this'),
                        $compilationContext
                    );
                    continue;
                }

                /**
                 * Resolve the value from the enclosing variable, never from the
                 * clone: only the enclosing one knows how it is actually held
                 * (a native `zend_string *` companion zval, a reference, ...).
                 */
                $enclosing = $compilationContext->symbolTable->getVariable($name);

                if ($var->isClosureReference()) {
                    $compilationContext->backend->updateClosureReferenceCapture(
                        $carrier,
                        $name,
                        $enclosing,
                        $compilationContext
                    );
                    continue;
                }

                $compilationContext->backend->updateClosureCapture(
                    $carrier,
                    $name,
                    $this->boxCapture($enclosing, $compilationContext),
                    $compilationContext
                );
            }
        }

        $symbolVariable = $this->generateClosure(
            $classDefinition,
            $classMethod,
            $block,
            $compilationContext,
            $expression,
            $bindThis,
            $carrier
        );

        ++self::$id;

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * A capture that is already a zval is written straight through:
     * getVariableCode() yields its address, and for a native `zend_string *`
     * parameter it yields the companion `<name>_zv` the parameter prologue
     * always populates. Boxing such a capture a second time emitted
     * ZVAL_STRING() on a zval, which does not compile. Only true C scalars
     * need a boxing temp.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2638
     */
    private function boxCapture(Variable $var, CompilationContext $compilationContext): Variable
    {
        $type = $var->getType();

        if (!in_array($type, ['int', 'uint', 'long', 'ulong', 'char', 'uchar', 'double', 'bool'], true)) {
            return $var;
        }

        $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
            'variable',
            $compilationContext,
            true
        );

        $backend = $compilationContext->backend;

        match ($type) {
            'double' => $backend->assignDouble($tempVariable, $var, $compilationContext),
            'bool'   => $backend->assignBool($tempVariable, $var, $compilationContext),
            default  => $backend->assignLong($tempVariable, $var, $compilationContext),
        };

        return $tempVariable;
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
        bool $bindThis = false,
        ?Variable $carrier = null
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
        $compilationContext->backend->createClosure(
            $symbolVariable,
            $classDefinition,
            $compilationContext,
            $bindThis,
            $carrier
        );

        return $symbolVariable;
    }
}
