<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation\File;

use Zephir\Documentation\FileInterface;
use Zephir\Documentation\NamespaceAccessor;

class NamespacesFile implements FileInterface
{
    public function __construct(protected NamespaceAccessor $namespaceAccessor)
    {
    }

    public function getTemplateName(): string
    {
        return 'namespaces.phtml';
    }

    public function getData(): array
    {
        return [
            'namespaceAccessor' => $this->namespaceAccessor,
            'namespacesTree' => $this->namespaceAccessor->getNamespaceTree(),
            'namespaces' => $this->namespaceAccessor->getByNamespace(),
        ];
    }

    public function getOutputFile(): string
    {
        return 'namespaces.html';
    }
}
