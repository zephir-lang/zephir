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

namespace Zephir\Documentation\File;

use Zephir\Documentation;
use Zephir\Documentation\FileInterface;
use Zephir\Documentation\NamespaceHelper;

class NamespaceFile implements FileInterface
{
    public function __construct(protected NamespaceHelper $namespaceHelper)
    {
    }

    public function getData(): array
    {
        return [
            'namespaceHelper' => $this->namespaceHelper,
            'subNamespaces'   => $this->namespaceHelper->getNamespaces(),
            'subClasses'      => $this->namespaceHelper->getClasses(),
        ];
    }

    public function getOutputFile(): string
    {
        return Documentation::namespaceUrl($this->namespaceHelper->getFullNamespace());
    }

    public function getTemplateName(): string
    {
        return 'namespace.phtml';
    }
}
