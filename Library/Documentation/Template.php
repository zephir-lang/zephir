<?php
/**
 * Created by PhpStorm.
 * User: bob
 * Date: 5/17/14
 * Time: 7:53 AM
 */

namespace Zephir\Documentation;


use Zephir\Exception;

class Template {

    protected $template;
    protected $rootDirectory;
    protected $data;
    protected $nestedLevel;
    protected $pathToRoot="./";

    function __construct($data, $rootDirectory, $template, $nestedLevel=0)
    {

        // todo clean buffer before exception
        if($nestedLevel>800)
            throw new Exception("Recursive inclusion detected in theme creation");

        // todo : securise parent inclusion
        $rootDirectory = rtrim($rootDirectory,"/");

        $this->data = $data;
        $this->rootDirectory = $rootDirectory;
        $this->template = $template;
        $this->nestedLevel = $nestedLevel;
    }

    /**
     * @param string $pathToRoot
     */
    public function setPathToRoot($pathToRoot)
    {
        $this->pathToRoot = $pathToRoot;
    }

    /**
     * @return string
     */
    public function getPathToRoot()
    {
        return $this->pathToRoot;
    }

    public function asset($name){
        return $this->getPathToRoot() . "asset/" . rtrim($name);
    }

    public function write($outputFile){

        $content = $this->parse();

        file_put_contents($outputFile,$content);

    }


    public function parse(){

        foreach($this->data as $name=>$value){
            $$name = $value;
        }

        ob_start();

        include($this->__getSecureFilePath($this->template) );

        $content = ob_get_clean();

        return $content;

    }

    private function __getSecureFilePath($fileName){

        $input   = pathinfo( $this->rootDirectory . "/" . $fileName);
        $inputDirname  = $input["dirname"];
        $inputBasename = $input["basename"];
        $inputFilename = $inputDirname . "/" . $inputBasename;

        if(!file_exists($inputFilename)){
            throw new Exception("Template not found : $inputFilename");
        }

        return $inputFilename;
    }


    public function partial($fileName,array $data=array()){

        $newLevel = $this->nestedLevel+1;

        $template = new Template(array_merge($this->data,$data),$this->rootDirectory,$fileName,$newLevel);
        $template->setPathToRoot($this->getPathToRoot());

        return $template->parse();
    }

} 