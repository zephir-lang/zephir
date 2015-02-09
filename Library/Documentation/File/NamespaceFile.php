<?php

namespace Zephir\Documentation\File;


use Zephir\ClassDefinition;
use Zephir\Documentation\AbstractFile;
use Zephir\CompilerFile;
use Zephir\Documentation\NamespaceHelper;

class NamespaceFile extends AbstractFile
{
    
    /**
     * @var \Zephir\Documentation\NamespaceHelper
     */
    protected $namespaceHelper;
    
    /**
     *
     * @var CompilerFile
     */
    protected $compilerFile;
    
    public function __construct($config, NamespaceHelper $nh)
    {
        $this->namespaceHelper = $nh;
    }

    public function getTemplateName()
    {
        return "namespace.phtml";
    }

    public function getData()
    {
     
        
        return array(

            "namespaceHelper" => $this->namespaceHelper,
            "subNamespaces" => $this->namespaceHelper->getNamespaces(),
            "subClasses" => $this->namespaceHelper->getClasses(),

        );
    }

    public function getOutputFile()
    {
        return \Zephir\Documentation::namespaceUrl($this->namespaceHelper->getFullNamespace());
    }
}
