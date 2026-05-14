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

use function explode;

class NamespaceAccessor
{
    protected array $byNamespace = [];

    protected array $namespaceTree = [];

    public function __construct(protected array $classes)
    {
    }

    public function build(): void
    {
        $byNamespace = [];
        $tree        = [];

        foreach ($this->classes as $class) {
            $ns        = explode('\\', $class->getClassDefinition()->getNamespace());
            $actualStr = '';
            foreach ($ns as $n) {
                if ($actualStr !== '') {
                    $previous  = $byNamespace[$actualStr];
                    $actualStr .= '\\';
                    $isRoot    = false;
                } else {
                    $previous = null;
                    $isRoot   = true;
                }
                $actualStr .= $n;

                if (!isset($byNamespace[$actualStr])) {
                    $byNamespace[$actualStr] = new NamespaceHelper($actualStr);
                }

                $nh = $byNamespace[$actualStr];

                if ($previous) {
                    $previous->addNamespace($nh);
                }

                if ($isRoot) {
                    if (!isset($tree[$n])) {
                        $tree[$n] = $nh;
                    }
                }
            }

            $nh->addClass($class);
        }

        $this->byNamespace   = $byNamespace;
        $this->namespaceTree = $tree;
    }

    /**
     * @return NamespaceHelper[]
     */
    public function getByNamespace(): array
    {
        return $this->byNamespace;
    }

    public function getNamespaceTree(): array
    {
        return $this->namespaceTree;
    }
}
