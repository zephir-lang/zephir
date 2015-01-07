<?php

namespace Zephir\Documentation;

use Zephir\CompilerFile;

class NamespaceHelper {
    
    protected $fullNamespace;
    protected $classes = array();
    protected $namespaces = array();
    
    function __construct($fullNamespace) {
        $this->fullNamespace = $fullNamespace;
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

    public function getClasses() {
        return $this->classes;
    }

    public function getNamespaces() {
        return $this->namespaces;
    }


    


    
}
