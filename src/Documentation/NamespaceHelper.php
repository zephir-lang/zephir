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

namespace Zephir\Documentation;

use Zephir\CompilerFile;

use function array_pop;
use function count;
use function explode;
use function implode;

class NamespaceHelper
{
    protected array  $classes    = [];
    protected string $endNamespace;
    protected array  $namespaces = [];
    protected string $parentName;

    public function __construct(protected string $fullNamespace)
    {
        $ex                 = explode('\\', $fullNamespace);
        $this->endNamespace = $ex[count($ex) - 1];

        array_pop($ex);

        $this->parentName = implode('\\', $ex);
    }

    public function addClass(CompilerFile $c): void
    {
        $this->classes[] = $c;
    }

    public function getClasses(): array
    {
        return $this->classes;
    }

    public function getFullNamespace(): string
    {
        return $this->fullNamespace;
    }

    public function getNamespaces(): array
    {
        return $this->namespaces;
    }

    public function getParentName(): string
    {
        return $this->parentName;
    }

    public function getShortName(): string
    {
        return $this->endNamespace;
    }
}
