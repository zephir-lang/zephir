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
use Zephir\FunctionDefinition;
use Zephir\StatementsBlock;
use Zephir\Variable\Variable;

use function in_array;
use function is_array;
use function sprintf;

/**
 * Creates an anonymous function within the extension simulating a closure
 */
class Closure
{
    /**
     * Native C scalar types. A capture of one of them is boxed into a zval
     * that holds no reference: a long, a double or a bool.
     */
    public const SCALAR_CAPTURE_TYPES = ['int', 'uint', 'long', 'ulong', 'char', 'uchar', 'double', 'bool'];

    protected bool $expecting = true;

    protected ?Variable $expectingVariable = null;
    /**
     * Unique closure ID.
     */
    protected static int $id = 0;

    protected bool $readOnly = false;

    /**
     * Restarts closure class numbering for a fresh compilation run, so a
     * closure's class name depends on the source, not on how many
     * compilations the process ran before.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2687
     */
    public static function reset(): void
    {
        self::$id = 0;
    }

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
     * Whether an AST node calls a method through `self::`, `parent::` or
     * `static::`: PHP passes the closure's $this on to such a call, so a
     * closure written in an instance method binds it, as PHP binds it there.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2825
     */
    protected static function astCallsClassScope(mixed $node): bool
    {
        if (!is_array($node)) {
            return false;
        }

        if (
            'scall' === ($node['type'] ?? null)
            && in_array($node['class'] ?? null, ['self', 'parent', 'static'], true)
        ) {
            return true;
        }

        foreach ($node as $child) {
            if (is_array($child) && self::astCallsClassScope($child)) {
                return true;
            }
        }

        return false;
    }

    /**
     * The user class a closure body's `this` belongs to.
     *
     * A closure declared inside another closure compiles with the *outer
     * closure's* synthetic class as the current scope, and that class owns no
     * user properties or methods. Every consumer of the enclosing definition
     * (property access, method call, `let this->x`, `return this`) resolves it
     * with a single hop, so the chain has to be flattened here, at the one
     * place it is written.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2655
     */
    protected static function enclosingUserClass(Definition $scope): ?Definition
    {
        while ($scope->isClosure()) {
            $scope = $scope->getEnclosingClassDefinition();
            if (null === $scope) {
                // The outermost closure is written in a function.
                return null;
            }
        }

        return $scope;
    }

    /**
     * Creates and registers the anonymous class a closure compiles to.
     *
     * The body is compiled later in a context of its own, so it has to carry
     * the name-resolution scope of the code it is written in: the `use ... as`
     * aliases (through the context handed to the anonymous file) and the
     * namespace. The class itself always lives in the extension namespace.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2686
     */
    protected function createClosureClass(CompilationContext $compilationContext): Definition
    {
        $classDefinition = new Definition(
            $compilationContext->config->get('namespace'),
            self::$id . '__closure'
        );

        $classDefinition->setIsFinal(true);
        $classDefinition->setIsClosure(true);
        $classDefinition->setSourceNamespace($compilationContext->currentNamespace());

        /**
         * The body is compiled later, on its own, so it keeps the `use`
         * aliases of the code it is written in, a merged trait method's too.
         */
        $aliasManager = $compilationContext->currentAliasManager();
        if (null !== $aliasManager) {
            $classDefinition->setAliasManager($aliasManager);
        }

        /**
         * The body runs in the scope of the class it is written in, whether or
         * not it uses `this`: PHP checks every private and protected member it
         * reaches against that class. A closure in a function has no class.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2734
         */
        $enclosing = null === $compilationContext->classDefinition
            ? null
            : self::enclosingUserClass($compilationContext->classDefinition);
        if (null !== $enclosing) {
            $classDefinition->setEnclosingClassDefinition($enclosing);
        }

        $compilerFile = new CompilerFileAnonymous($classDefinition, $compilationContext->config, $compilationContext);
        $compilerFile->setLogger($compilationContext->logger);

        $compilationContext->compiler->addClassDefinition($compilerFile, $classDefinition);

        return $classDefinition;
    }

    /**
     * Creates a closure.
     *
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $classDefinition = $this->createClosureClass($compilationContext);

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
                 * Both clause flags belong to this one clause, not to the
                 * variable: the same local can be captured by value by one
                 * closure and by reference by another, and only the clause
                 * that says `const` makes its capture read only.
                 *
                 * Assigned rather than OR-ed with the captured variable's own
                 * flags on purpose. A by-value capture is a private copy, so
                 * `use (x)` of a `const` parameter is writable inside the body
                 * exactly as PHP's `use ($x)` is.
                 *
                 * @see https://github.com/zephir-lang/zephir/issues/2652
                 * @see https://github.com/zephir-lang/zephir/issues/2653
                 */
                $capture = clone $captured;
                $capture->setIsClosureReference(!empty($parameter['reference']));
                $capture->setReadOnly(!empty($parameter['const']));

                $staticVariables[$parameter['name']] = $capture;
            }
        }

        /**
         * Detect if the closure body references `this`.
         * If so, we need to bind the enclosing object to the closure.
         */
        $bindThis = self::astReferencesThis($block)
            || (
                !$compilationContext->staticContext
                && null !== $compilationContext->lexicalClassDefinition()
                && self::astCallsClassScope($block)
            );
        if ($bindThis) {
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

        if (!in_array($type, self::SCALAR_CAPTURE_TYPES, true)) {
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

        [$legacyName, $modernName] = self::closureNames($expression, $compilationContext);
        $classDefinition->setClosureNames($legacyName, $modernName);
        $classDefinition->setClosureTrait(
            $compilationContext->classDefinition?->isClosure()
                ? $compilationContext->classDefinition->getClosureTrait()
                : $compilationContext->declaringTraitDefinition()?->getCompleteName()
        );

        return $symbolVariable;
    }

    /**
     * The names PHP gives a closure written here: `Ns\{closure}` before PHP
     * 8.4, and from 8.4 on `{closure:<where>:<line>}`, where it is written
     * being the method as `Class::method()` (the trait, for a method merged
     * from one), the function as `ns\fn()`, or the enclosing closure's own
     * name, and the line the one it starts on.
     *
     * @return array{0: string, 1: string}
     *
     * @see https://github.com/zephir-lang/zephir/issues/2825
     */
    private static function closureNames(array $expression, CompilationContext $compilationContext): array
    {
        $method   = $compilationContext->currentMethod;
        $function = $method instanceof FunctionDefinition ? $method : null;

        $namespace  = null !== $function && $function->isGlobal() ? '' : $compilationContext->currentNamespace();
        $legacyName = ('' === $namespace ? '' : $namespace . '\\') . '{closure}';

        if (true === $compilationContext->classDefinition?->isClosure()) {
            $where = (string) $compilationContext->classDefinition->getClosureNames()[1];
        } elseif (null !== $function) {
            $where = ($function->isGlobal() ? '' : $function->getNamespace() . '\\') . $function->getName() . '()';
        } else {
            $class = $compilationContext->declaringTraitDefinition() ?? $compilationContext->lexicalClassDefinition();
            $where = $class->getCompleteName() . '::' . $method->getDeclaredName() . '()';
        }

        $line = $expression['start-line'] ?? $expression['line'];

        return [$legacyName, sprintf('{closure:%s:%d}', $where, $line)];
    }
}
