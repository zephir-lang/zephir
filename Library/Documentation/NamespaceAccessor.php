<?php

namespace Zephir\Documentation;

use Zephir\Documentation\NamespaceHelper;

class NamespaceAccessor {
    
    /**
     *
     * @var \Zephir\CompilerFile[]
     */
    protected $classes;
    
    protected $namespaceTree;
    protected $byNamespace;


    /**
     * 
     * @param \Zephir\CompilerFile[] $files
     */
    function __construct($files) {
        $this->classes = $files;
    }
    
    /**
     * 
     * @return NamespaceHelper[]
     */
    public function getByNamespace() {
        return $this->byNamespace;
    }

    public function getNamespaceTree() {
        return $this->namespaceTree;
    }

        
    function build(){
        
        
        $byNamespace = array();
        $tree = array();
        
        foreach($this->classes as $className => $class){
            
            $ns = explode("\\",$class->getClassDefinition()->getNamespace());
            $actualStr = "";
            foreach ($ns as $n){
                
                if(strlen($actualStr) > 0){
                    $previous = $byNamespace[$actualStr];
                    $actualStr.= "\\";
                    $isRoot = true;
                }else{
                    $previous = null;
                    $isRoot = false;
                }
                $actualStr.= $n;
                
                if(!isset($byNamespace[$actualStr])){
                    $byNamespace[$actualStr] = new NamespaceHelper($actualStr);
                }
                
                $nh = $byNamespace[$actualStr];
                
                if($previous){
                    $previous->addNamespace($nh);
                }
                
                if($isRoot){
                    if(!isset($tree[$n])){
                        $tree[$n] = $nh;
                    }
                }
                
            }
            
            $nh->addClass($class);
            
        }
        
        $this->byNamespace = $byNamespace;
        
    }
    
}
