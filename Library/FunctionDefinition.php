<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

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
    private $namespace;

    /**
     * Whether the function is declared in a global or namespaced scope.
     *
     * @var bool
     */
    private $isGlobal;

    public function __construct(
        $namespace,
        $name,
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
    public function getInternalName()
    {
        return ($this->isGlobal() ? 'g_' : 'f_').str_replace('\\', '_', $this->namespace).'_'.$this->getName();
    }

    public function isGlobal()
    {
        return $this->isGlobal;
    }

    public function setGlobal($global)
    {
        $this->isGlobal = $global;
    }

    public function getNamespace()
    {
        return $this->namespace;
    }

    public function setNamespace($namespace)
    {
        $this->namespace = $namespace;
    }

    public function hasModifier($modifier)
    {
        return false;
    }

    public function getVisibility()
    {
        return [];
    }
}
