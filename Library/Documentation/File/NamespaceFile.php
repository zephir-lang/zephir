<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation\File;

use Zephir\CompilerFile;
use Zephir\Documentation\AbstractFile;
use Zephir\Documentation\NamespaceHelper;

class NamespaceFile extends AbstractFile
{
    /**
     * @var \Zephir\Documentation\NamespaceHelper
     */
    protected $namespaceHelper;

    /**
     * @var CompilerFile
     */
    protected $compilerFile;

    public function __construct($config, NamespaceHelper $nh)
    {
        $this->namespaceHelper = $nh;
    }

    public function getTemplateName()
    {
        return 'namespace.phtml';
    }

    public function getData()
    {
        return [
            'namespaceHelper' => $this->namespaceHelper,
            'subNamespaces' => $this->namespaceHelper->getNamespaces(),
            'subClasses' => $this->namespaceHelper->getClasses(),
        ];
    }

    public function getOutputFile()
    {
        return \Zephir\Documentation::namespaceUrl($this->namespaceHelper->getFullNamespace());
    }
}
