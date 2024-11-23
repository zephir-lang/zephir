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

use Zephir\Class\Method\Method;
use Zephir\Class\Method\Parameters;

/**
 * Represents a function
 */
class FunctionDefinition extends Method
{
    /**
     * Whether the function is declared in a global or namespaced scope.
     */
    private bool $isGlobal = false;

    public function __construct(
        private string $namespace,
        protected string $name,
        protected ?Parameters $parameters = null,
        protected ?StatementsBlock $statements = null,
        ?array $returnType = null,
        protected ?array $expression = [],
    ) {
        $this->setReturnTypes($returnType);
    }

    /**
     * Get the internal name used in generated C code.
     */
    public function getInternalName(): string
    {
        return ($this->isGlobal() ? 'g_' : 'f_') . str_replace('\\', '_', $this->namespace) . '_' . $this->getName();
    }

    public function getNamespace(): string
    {
        return $this->namespace;
    }

    public function getVisibility(): array
    {
        return [];
    }

    public function hasModifier(string $modifier): bool
    {
        return false;
    }

    public function isGlobal(): bool
    {
        return $this->isGlobal;
    }

    public function setGlobal(bool $global): void
    {
        $this->isGlobal = $global;
    }

    public function setNamespace(string $namespace): void
    {
        $this->namespace = $namespace;
    }
}
