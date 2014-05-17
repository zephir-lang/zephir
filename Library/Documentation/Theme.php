<?php
/**
 * Created by PhpStorm.
 * User: bob
 * Date: 5/17/14
 * Time: 7:58 AM
 */

namespace Zephir\Documentation;


use Zephir\Exception;

class Theme {

    protected $themeDir;
    protected $outputDir;
    protected $themeConfig;

    function __construct($themeDir , $outputDir , $themeConfig)
    {
        $this->outputDir   = $outputDir;
        $this->themeConfig = $themeConfig;
        $this->themeDir    = $themeDir;
    }


    public function drawFile(AbstractFile $file){

        $output   = pathinfo( $this->outputDir . "/" . $file->getOutputFile());
        $outputDirname  = $output["dirname"];
        $outputBasename = $output["basename"];
        $outputFilename = $outputDirname . "/" . $outputBasename;


        // todo : check if writable
        if(!file_exists($outputDirname))
            mkdir($outputDirname,0777,true);


        $template = new Template($file->getData() , $this->themeDir , $file->getTemplateName());

        touch($outputFilename);
        $template->write($outputFilename);


    }

} 