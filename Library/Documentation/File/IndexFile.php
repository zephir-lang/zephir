<?php

namespace Zephir\Documentation\File;

use Zephir\Documentation\AbstractFile;
use Zephir\Documentation\NamespaceAccessor;

class IndexFile extends AbstractFile
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
        return "index.phtml";
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
        return "index.html" ;
    }
}
