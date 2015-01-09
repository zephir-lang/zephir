<?php

namespace Zephir\Documentation;

use Zephir\CompilerFile;

class NamespaceHelper {
    
    protected $fullNamespace;

    protected $endNamespace;
    protected $parentName;

    protected $classes = array();
    protected $namespaces = array();
    
    function __construct($fullNamespace) {
        $this->fullNamespace = $fullNamespace;

        $ex = explode("\\",$fullNamespace);
        $this->endNamespace = $ex[count($ex) - 1];

        array_pop($ex);

        $this->parentName = implode("\\",$ex);

    }
    
    public function addClass(CompilerFile $c){
        $this->classes[] = $c;
    }
    
    public function addNamespace(NamespaceHelper $n){
        if(!isset($this->namespaces[$n->getFullNamespace()]))
            $this->namespaces[$n->getFullNamespace()] = $n;
    }
    
    public function getFullNamespace() {
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
    public function getClasses() {
        return $this->classes;
    }

    /**
     * @return NamespaceHelper[]
     */
    public function getNamespaces() {
        return $this->namespaces;
    }


    


    
}
