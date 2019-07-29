<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation;

use Zephir\CompilerFile;

class NamespaceHelper
{
    protected $fullNamespace;

    protected $endNamespace;
    protected $parentName;

    protected $classes = [];
    protected $namespaces = [];

    public function __construct($fullNamespace)
    {
        $this->fullNamespace = $fullNamespace;

        $ex = explode('\\', $fullNamespace);
        $this->endNamespace = $ex[\count($ex) - 1];

        array_pop($ex);

        $this->parentName = implode('\\', $ex);
    }

    public function addClass(CompilerFile $c)
    {
        $this->classes[] = $c;
    }

    public function addNamespace(self $n)
    {
        if (!isset($this->namespaces[$n->getFullNamespace()])) {
            $this->namespaces[$n->getFullNamespace()] = $n;
        }
    }

    public function getFullNamespace()
    {
        return $this->fullNamespace;
    }

    /**
     * @return string
     */
    public function getShortName()
    {
        return $this->endNamespace;
    }

    /**
     * @return string
     */
    public function getParentName()
    {
        return $this->parentName;
    }

    /**
     * @return CompilerFile[]
     */
    public function getClasses()
    {
        return $this->classes;
    }

    /**
     * @return NamespaceHelper[]
     */
    public function getNamespaces()
    {
        return $this->namespaces;
    }
}
