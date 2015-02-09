<?php

namespace Zephir\Documentation\File;

use Zephir\Documentation\AbstractFile;
use Zephir\Documentation\NamespaceAccessor;

class NamespacesFile extends AbstractFile
{

    /**
     * @var NamespaceAccessor
     */
    protected $namespaceAccessor;

    public function __construct($config, NamespaceAccessor $namespaceAccessor)
    {
        $this->namespaceAccessor = $namespaceAccessor;
    }

    public function getTemplateName()
    {
        return "namespaces.phtml";
    }

    public function getData()
    {
        return array(
            "namespaceAccessor" => $this->namespaceAccessor,
            "namespacesTree" => $this->namespaceAccessor->getNamespaceTree(),
            "namespaces" => $this->namespaceAccessor->getByNamespace()
        );
    }

    public function getOutputFile()
    {
        return "namespaces.html" ;
    }
}
