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

namespace Zephir\Class\Definition;

use Zephir\Class\Attribute;
use Zephir\Class\Constant;
use Zephir\Class\Entry;
use Zephir\Class\Method\Method;
use Zephir\Class\Property;
use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;
use Zephir\FunctionDefinition;
use Zephir\Expression\ConstantExpressionEvaluator;
use Zephir\Name;

use function in_array;
use function sprintf;
use function strtolower;

/**
 * Emits the MINIT-time calls that attach the `#[Foo(...)]` records declared in
 * .zep source to a Zephir class and its members (issue #2466).
 *
 * Why MINIT and not a static table: the `zend_internal_arg_info[]` array and
 * the ZEPHIR_INIT_FUNCS method entry in the generated header have no attribute
 * slot at all. A method attribute lives on the `zend_function` and a parameter
 * attribute on `func->common.attributes` at `offset + 1`, so every target —
 * class, method, parameter, property, class constant — is attached from
 * ZEPHIR_INIT_CLASS, once the register macro has created the class entry.
 *
 * The attribute class itself is never looked up. The engine stores a plain
 * name and resolves it lazily at reflection time, so an attribute may name a
 * userland PHP class the compiler has never seen — exactly as in PHP.
 *
 * A collaborator rather than more methods on Definition, following the
 * precedent set by TraitMerger.
 */
final class AttributeEmitter
{
    /**
     * What a reduced attribute argument may be: everything the engine can hold
     * as a persistent attribute-argument value, and nothing else.
     *
     * `char` is deliberately absent. A Zephir char is a byte value, and PHP has
     * no such scalar, so silently boxing it as a one-character string would be
     * a guess (see zephir#2069, closed as by-design).
     */
    private const ARGUMENT_TYPES = [
        'int',
        'uint',
        'long',
        'ulong',
        'double',
        'float',
        'bool',
        'null',
        'string',
        'istring',
        'array',
        'empty-array',
    ];

    /**
     * PHP attributes whose behaviour comes from a compile-time validator in
     * Zend/zend_attributes.c. That validator only runs while the engine
     * compiles userland code, so for a class registered at MINIT attaching the
     * record is not enough — the matching ZEND_ACC_* flag has to be set too,
     * exactly as gen_stub.php does for php-src's own internal classes.
     *
     * Keyed by the resolved global name, lowercased: PHP matches an attribute
     * class name case-insensitively, and these live in the global namespace, so
     * `#[Deprecated]` inside `namespace Stub;` is Stub\Deprecated and must not
     * match. The kernel macro is 0 on the versions where the attribute does not
     * exist, which keeps Zephir from being more deprecating than its host PHP.
     */
    private const FLAG_ATTRIBUTES = [
        'class' => [
            'allowdynamicproperties' => 'ZEPHIR_ATTR_ALLOW_DYNAMIC_PROPERTIES',
        ],
        'method' => [
            'deprecated' => 'ZEPHIR_ATTR_DEPRECATED',
            'nodiscard'  => 'ZEPHIR_ATTR_NODISCARD',
        ],
        'constant' => [
            'deprecated' => 'ZEPHIR_ATTR_DEPRECATED',
        ],
    ];

    public function __construct(private CompilationContext $context)
    {
    }

    /**
     * The class, interface or trait itself, plus every method and parameter.
     *
     * Emitted right after the register macro, i.e. BEFORE the
     * zend_class_implements() calls, because the engine's
     * interface-implementation check reads the child method's attribute table
     * looking for `returntypewillchange`. Attaching first is the only way
     * `#[\ReturnTypeWillChange]` can suppress the tentative-return-type
     * deprecation a Zephir class gets from Countable, ArrayAccess and friends.
     */
    public function emitClassLevel(Definition $definition): void
    {
        $entry   = $definition->getClassEntry($this->context);
        $printed = false;

        foreach ($definition->getAttributes() as $attribute) {
            $name = $this->emit(sprintf('zephir_add_class_attribute(%s', $entry), $attribute);
            $this->emitFlag('class', $name, sprintf('zephir_mark_class_flags(%s', $entry));
            $printed = true;
        }

        foreach ($definition->getMethods() as $method) {
            /**
             * An internal method is a plain C function: the ZEPHIR_INIT_FUNCS
             * table skips it, so it is absent from ce->function_table and
             * cannot carry an attribute. Skipping it also keeps the
             * internal-call twin, a generator's step function and the property
             * initializer — all of which share the declaring method's raw node
             * — from attaching the same attribute a second time.
             */
            if ($method->isInternal()) {
                continue;
            }

            /**
             * ce->function_table is keyed by the lowercased name, while Zephir
             * keeps the source spelling in PHP_ME.
             */
            $lcName = strtolower($method->getName());

            foreach ($method->getAttributes() as $attribute) {
                $name = $this->emit(
                    sprintf('zephir_add_method_attribute(%s, SL("%s")', $entry, $lcName),
                    $attribute
                );
                $this->checkOverride($definition, $name, 'method', $method->getName(), $attribute);
                $this->checkNoDiscard($definition, $name, $method, $attribute);
                $this->emitFlag(
                    'method',
                    $name,
                    sprintf('zephir_mark_method_flags(%s, SL("%s")', $entry, $lcName)
                );
                $printed = true;
            }

            $parameters = $method->getParameters();
            if (null === $parameters) {
                continue;
            }

            /**
             * The index is the declared position, which is the order arg_info
             * is rendered in and therefore the offset ReflectionParameter
             * matches on. zend_add_parameter_attribute() applies the engine's
             * `+ 1` itself.
             */
            foreach ($parameters->getParameters() as $offset => $parameter) {
                foreach (Attribute::listFromNode($parameter) as $attribute) {
                    $this->emit(
                        sprintf('zephir_add_parameter_attribute(%s, SL("%s"), %d', $entry, $lcName, $offset),
                        $attribute
                    );
                    $printed = true;
                }
            }
        }

        if ($printed) {
            $this->context->codePrinter->outputBlankLine();
        }
    }

    /**
     * A top-level function and its parameters.
     *
     * Emitted into MINIT like everything else: the engine registers a module's
     * functions in zend_register_module_ex(), which runs before
     * zend_startup_module_ex() calls MINIT, so CG(function_table) is already
     * populated. A namespaced function is interned there as `<ns>\<name>`,
     * lowercased.
     */
    public function emitFunction(FunctionDefinition $function): void
    {
        $lcName = strtolower(
            $function->isGlobal()
                ? $function->getName()
                : $function->getNamespace() . '\\' . $function->getName()
        );
        /**
         * Entry::escape(), not Name::addSlashes(): the latter preserves a
         * recognized escape sequence, so a namespace segment beginning with
         * one (`App\...` reads as `\a`, `Tools\...` as `\t`) would reach C as
         * a control character and the lookup would silently miss.
         */
        $escaped = Entry::escape($lcName);

        foreach ($function->getAttributes() as $attribute) {
            $name = $this->emit(
                sprintf('zephir_add_function_attribute(SL("%s")', $escaped),
                $attribute,
                $function->getNamespace()
            );
            $this->checkNoDiscard(null, $name, $function, $attribute);
            $this->emitFlag('method', $name, sprintf('zephir_mark_function_flags(SL("%s")', $escaped));
        }

        $parameters = $function->getParameters();
        if (null === $parameters) {
            return;
        }

        foreach ($parameters->getParameters() as $offset => $parameter) {
            foreach (Attribute::listFromNode($parameter) as $attribute) {
                $this->emit(
                    sprintf('zephir_add_function_parameter_attribute(SL("%s"), %d', $escaped, $offset),
                    $attribute,
                    $function->getNamespace()
                );
            }
        }
    }

    /**
     * Must follow the property's declare: the helper needs the
     * zend_property_info the declare created.
     */
    public function emitProperty(Definition $definition, Property $property): void
    {
        $entry = $definition->getClassEntry($this->context);

        foreach ($property->getAttributes() as $attribute) {
            $name = $this->emit(
                sprintf('zephir_add_property_attribute(%s, SL("%s")', $entry, $property->getName()),
                $attribute
            );
            $this->checkOverride($definition, $name, 'property', $property->getName(), $attribute);
            $this->context->codePrinter->outputBlankLine();
        }
    }

    /**
     * Must follow the constant's declare, for the same reason as a property.
     */
    public function emitConstant(Definition $definition, Constant $constant): void
    {
        $entry = $definition->getClassEntry($this->context);

        foreach ($constant->getAttributes() as $attribute) {
            $name = $this->emit(
                sprintf('zephir_add_class_constant_attribute(%s, SL("%s")', $entry, $constant->getName()),
                $attribute
            );
            $this->checkOverride($definition, $name, 'constant', $constant->getName(), $attribute);
            $this->emitFlag(
                'constant',
                $name,
                sprintf('zephir_mark_class_constant_flags(%s, SL("%s")', $entry, $constant->getName())
            );
        }
    }

    /**
     * @param string $target the `zephir_add_*_attribute(<target>` prefix
     *
     * @return string the resolved, unescaped FQCN of the attribute
     */
    private function emit(string $target, Attribute $attribute, ?string $namespace = null): string
    {
        $arguments = [];
        foreach ($attribute->getArguments() as $argument) {
            $arguments[] = [
                'name' => $argument['name'],
                'expr' => $this->reduceArgument($argument['expr']),
            ];
        }

        /**
         * Fold in place, so the stub generator can render `#[Foo(2 * 3)]` as
         * `#[Foo(6)]`: an expression node has no PHP spelling.
         */
        $attribute->setReducedArguments($arguments);

        $name = $this->resolveName($attribute->getName(), $namespace);

        $this->context->backend->declareAttribute(
            $target,
            Entry::escape($name),
            $arguments,
            $this->context
        );

        return $name;
    }

    /**
     * Sets the ZEND_ACC_* flag the engine's own validator would have set.
     */
    private function emitFlag(string $kind, string $name, string $target): void
    {
        $flag = self::FLAG_ATTRIBUTES[$kind][strtolower($name)] ?? null;

        if (null === $flag) {
            return;
        }

        $this->context->codePrinter->output(sprintf('%s, %s);', $target, $flag));
    }

    /**
     * Reports a `#[\Override]` that has nothing to override, the way PHP's own
     * validator does — but only when the whole hierarchy is visible here.
     *
     * PHP checks against the real classes and always has them. Zephir may not:
     * a parent or interface that is neither a Zephir class nor loadable by
     * reflection is invisible at compile time, and rejecting correct code is
     * worse than missing the diagnostic, so the check is skipped there.
     *
     * The ZEND_ACC_OVERRIDE flag is deliberately not co-emitted: the engine
     * checks it in a user-class-only path that asserts the function is not
     * internal, so setting it on a Zephir method would trip that assert in a
     * debug build.
     */
    private function checkOverride(
        Definition $definition,
        string $name,
        string $kind,
        string $member,
        Attribute $attribute
    ): void {
        if ('override' !== strtolower($name)) {
            return;
        }

        if (!$definition->hasResolvableAncestors() || $definition->hasInheritedMember($kind, $member)) {
            return;
        }

        throw new CompilerException(
            sprintf(
                'method' === $kind
                    ? '%s::%s() has #[\Override] attribute, but no matching parent method exists'
                    : ('constant' === $kind
                        ? '%s::%s has #[\Override] attribute, but no matching parent constant exists'
                        : '%s::$%s has #[\Override] attribute, but no matching parent property exists'),
                $definition->getCompleteName(),
                $member
            ),
            $attribute->getOriginal()
        );
    }

    /**
     * `#[\NoDiscard]` requires a return value, so PHP refuses to compile it on
     * a `void` or `never` function. Reported here for the same reason.
     */
    private function checkNoDiscard(
        ?Definition $definition,
        string $name,
        Method $method,
        Attribute $attribute
    ): void {
        if ('nodiscard' !== strtolower($name) || !$method->isVoid()) {
            return;
        }

        throw new CompilerException(
            sprintf(
                'A void %s does not return a value, but #[\NoDiscard] requires a return value (%s)',
                null === $definition ? 'function' : 'method',
                null === $definition
                    ? $method->getName()
                    : $definition->getCompleteName() . '::' . $method->getName()
            ),
            $attribute->getOriginal()
        );
    }

    /**
     * Lexical resolution only, with PHP's own `zend_resolve_class_name`
     * semantics: a leading `\` is absolute, a leading segment may be a `use`
     * alias, and anything else is relative to the current namespace.
     */
    private function resolveName(string $name, ?string $namespace = null): string
    {
        return Name::fetchFQN(
            $name,
            $namespace ?? $this->context->classDefinition->getNamespace(),
            $this->context->aliasManager
        );
    }

    /**
     * @throws CompilerException
     */
    private function reduceArgument(array $expr): array
    {
        if ('char' === $expr['type']) {
            throw new CompilerException(
                'A char literal cannot be used as an attribute argument, because a char is a byte '
                . 'value; use a double-quoted string instead',
                $expr
            );
        }

        if (
            'static-constant-access' === $expr['type']
            && 'class' === ($expr['right']['value'] ?? null)
            && 'static' === ($expr['left']['value'] ?? null)
        ) {
            throw new CompilerException(
                'static::class is resolved by late static binding and is not a compile-time constant '
                . 'expression, so it cannot be used as an attribute argument',
                $expr
            );
        }

        $reduced = (new ConstantExpressionEvaluator())->reduce($expr, $this->context);

        if (!in_array($reduced['type'], self::ARGUMENT_TYPES, true)) {
            throw new CompilerException(
                sprintf(
                    'Expression of type "%s" is not a compile-time constant expression',
                    $reduced['type']
                ),
                $expr
            );
        }

        return $reduced;
    }
}
