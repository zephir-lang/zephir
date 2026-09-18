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

namespace Zephir\Class;

use function count;

/**
 * One `#[Foo(...)]` written in .zep source, on any of the targets PHP allows
 * for a class member: the class, interface or trait itself, a property, a class
 * constant, a method, or a method parameter.
 *
 * The parser appends `attributes` last on each host node and omits it entirely
 * when there is none, so a declaration without attributes keeps the exact node
 * shape it had before. That matters beyond tidiness: TraitMerger compares
 * member ASTs to decide whether two traits contribute the same member, and an
 * array comparison is key-order sensitive.
 *
 * `name` is kept exactly as written — possibly an alias, possibly relative.
 * Resolution is deferred to emit time, where the alias manager of the file
 * being compiled is on the CompilationContext, and because the stub generator
 * wants the source spelling anyway: a stub reproduces the same namespace and
 * the same `use` statements.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2466
 */
final class Attribute
{
    /**
     * Arguments after compile-time reduction, or null while still raw.
     *
     * Populated when the class initializer is written, exactly as
     * Property::$defaultValue and Constant::$value are folded in place. The
     * stub generator reads the reduced form because an expression argument such
     * as `#[Foo(2 * 3)]` has no PHP spelling as an AST node.
     *
     * @var list<array{name: string|null, expr: array}>|null
     */
    private ?array $reduced = null;

    /**
     * @param list<array{name: string|null, expr: array}> $arguments
     */
    private function __construct(
        private string $name,
        private array $arguments,
        private array $original,
    ) {
    }

    /**
     * Reads the `attributes` key off any host node. A node without one — the
     * common case — yields an empty list.
     *
     * @return self[]
     */
    public static function listFromNode(?array $node): array
    {
        $attributes = [];

        foreach ($node['attributes'] ?? [] as $attribute) {
            $arguments = [];

            /**
             * Arguments reuse the call-argument node, so `name` is the named
             * argument's label and `parameter` is the expression.
             */
            foreach ($attribute['arguments'] ?? [] as $argument) {
                $arguments[] = [
                    'name' => $argument['name'] ?? null,
                    'expr' => $argument['parameter'],
                ];
            }

            $attributes[] = new self($attribute['name'], $arguments, $attribute);
        }

        return $attributes;
    }

    public function getName(): string
    {
        return $this->name;
    }

    /**
     * The arguments as the emitters see them: reduced to literals once the
     * class initializer has been written.
     *
     * @return list<array{name: string|null, expr: array}>
     */
    public function getArguments(): array
    {
        return $this->reduced ?? $this->arguments;
    }

    /**
     * @param list<array{name: string|null, expr: array}> $arguments
     */
    public function setReducedArguments(array $arguments): void
    {
        $this->reduced = $arguments;
    }

    public function countArguments(): int
    {
        return count($this->arguments);
    }

    /**
     * The raw node, for a CompilerException's file/line.
     */
    public function getOriginal(): array
    {
        return $this->original;
    }
}
