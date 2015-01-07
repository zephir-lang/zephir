<?php

namespace Zephir\Documentation\File;


use Zephir\ClassDefinition;
use Zephir\Documentation\AbstractFile;
use Zephir\CompilerFile;

class ClassFile extends AbstractFile {

    /**
     * @var ClassDefinition
     */
    protected $class;
    
    /**
     *
     * @var CompilerFile
     */
    protected $compilerFile;
    
    function __construct($config , CompilerFile $class)
    {
        $this->compilerFile = $class;
        $this->class = $class->getClassDefinition();
    }

    public function getTemplateName()
    {
        return "class.phtml";
    }

    public function getData()
    {
        
        $nsPieces = explode('\\', $this->class->getNamespace());
        
        $nsPathes = array();
        $nsStr = "";
        
        foreach($nsPieces as $n){
            
            if(strlen($nsStr) > 0){
                $nsStr.= '\\';
            }
            $nsStr.= $n;
            $nsPathes[$n] = $nsStr;
        }
        
        return array(

            "classDefinition" => $this->class,
            "compilerFile"    => $this->compilerFile,
            "className" => $this->class->getName(),
            "classNamespace" => $this->class->getNamespace(),
            "fullName"  => $this->class->getCompleteName(),
            "methods"   => $this->class->getMethods(),
            "namespacePieces" => $nsPathes

        );
    }

    public function getOutputFile()
    {
        return \Zephir\Documentation::classUrl($this->class);
    }



} 