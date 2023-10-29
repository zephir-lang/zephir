<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation;

use Zephir\CompilerFile;

class NamespaceHelper
{
    protected string $endNamespace;
    protected string $parentName;

    protected array $classes = [];
    protected array $namespaces = [];

    public function __construct(protected string $fullNamespace)
    {
        $ex = explode('\\', $fullNamespace);
        $this->endNamespace = $ex[\count($ex) - 1];

        array_pop($ex);

        $this->parentName = implode('\\', $ex);
    }

    public function addClass(CompilerFile $c): void
    {
        $this->classes[] = $c;
    }

    public function getFullNamespace(): string
    {
        return $this->fullNamespace;
    }

    public function getShortName(): string
    {
        return $this->endNamespace;
    }

    public function getParentName(): string
    {
        return $this->parentName;
    }

    public function getClasses(): array
    {
        return $this->classes;
    }

    public function getNamespaces(): array
    {
        return $this->namespaces;
    }
}
