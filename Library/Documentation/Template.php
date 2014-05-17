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

    function __construct($data, $rootDirectory, $template)
    {

        // todo : securise parent inclusion
        $rootDirectory = rtrim($rootDirectory,"/");

        $this->data = $data;
        $this->rootDirectory = $rootDirectory;
        $this->template = $template;
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

        $template = new Template(array_merge($this->data,$data),$this->rootDirectory,$fileName);

        return $template->parse();
    }

} 