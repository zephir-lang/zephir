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

namespace Zephir;

/**
 * FunctionDefinition.
 *
 * Represents a function
 */
class FunctionDefinition extends ClassMethod
{
    /**
     * The namespace of the function.
     */
    private string $namespace;

    /**
     * Whether the function is declared in a global or namespaced scope.
     *
     * @var bool
     */
    private bool $isGlobal = false;

    /**
     * FunctionDefinition constructor.
     *
     * @param string $namespace
     * @param string $name
     * @param ClassMethodParameters|null $parameters
     * @param StatementsBlock|null $statements
     * @param array|null $returnType
     * @param array|null $expression
     */
    public function __construct(
        string $namespace,
        string $name,
        ClassMethodParameters $parameters = null,
        StatementsBlock $statements = null,
        array $returnType = null,
        array $expression = null
    ) {
        $this->namespace = $namespace;
        $this->name = $name;
        $this->parameters = $parameters;
        $this->statements = $statements;
        $this->expression = $expression;

        $this->setReturnTypes($returnType);
    }

    /**
     * Get the internal name used in generated C code.
     */
    public function getInternalName(): string
    {
        return ($this->isGlobal() ? 'g_' : 'f_').str_replace('\\', '_', $this->namespace).'_'.$this->getName();
    }

    public function isGlobal(): bool
    {
        return $this->isGlobal;
    }

    public function setGlobal(bool $global): void
    {
        $this->isGlobal = $global;
    }

    public function getNamespace(): string
    {
        return $this->namespace;
    }

    public function setNamespace(string $namespace): void
    {
        $this->namespace = $namespace;
    }

    public function hasModifier(string $modifier): bool
    {
        return false;
    }

    public function getVisibility(): array
    {
        return [];
    }
}
