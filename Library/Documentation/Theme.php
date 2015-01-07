<?php

namespace Zephir\Documentation;


use Zephir\Exception;

class Theme {

    protected $themeDir;
    protected $outputDir;
    protected $themeConfig;
    protected $options;
    
    function __construct($themeDir , $outputDir , $themeConfig)
    {
        $this->outputDir   = $outputDir;
        $this->themeConfig = $themeConfig;
        $this->themeDir    = $themeDir;
        $this->options     = $themeConfig["options"];
    }


    public function drawFile(AbstractFile $file){

        $outputFile = ltrim($file->getOutputFile() ,"/");
        
        $output   = pathinfo( $this->outputDir . "/" . $outputFile);
        $outputDirname  = $output["dirname"];
        $outputBasename = $output["basename"];
        $outputFilename = $outputDirname . "/" . $outputBasename;


        // todo : check if writable
        if(!file_exists($outputDirname))
            mkdir($outputDirname,0777,true);



        $subDirNumber= count(explode("/", $outputFile))-1;

        if($subDirNumber > 0)
            $pathToRoot = str_repeat("../",$subDirNumber);
        else
            $pathToRoot = "./";

        $template = new Template($file->getData() , $this->themeDir , $file->getTemplateName());
        $template->setPathToRoot($pathToRoot);
        $template->setThemeOptions($this->options);
        
        touch($outputFilename);
        $template->write($outputFilename);
    }

    public function buildStaticDirectory(){



        $themeStt = $this->getThemePath("static");

        if(file_exists($themeStt)){

            $outputStt = $this->getOutputPath("asset");

            if(!file_exists($outputStt))
                mkdir($outputStt);

            $this->__copyDir($themeStt,$outputStt . "/static");

        }

    }

    /**
     * from : http://stackoverflow.com/questions/2050859/copy-entire-contents-of-a-directory-to-another-using-php
     * @param $src
     * @param $dst
     */
    private function __copyDir($src,$dst){
        $dir = opendir($src);
        @mkdir($dst);
        while(false !== ( $file = readdir($dir)) ) {
            if (( $file != '.' ) && ( $file != '..' )) {
                if ( is_dir($src . '/' . $file) ) {
                    $this->__copyDir($src . '/' . $file,$dst . '/' . $file);
                }
                else {
                    copy($src . '/' . $file,$dst . '/' . $file);
                }
            }
        }
        closedir($dir);
    }

    public function getThemePath($path){

        $path   = pathinfo( $this->themeDir . "/" . $path);
        $pathDirname  = $path["dirname"];
        $pathBasename = $path["basename"];
        $pathFilename = $pathDirname . "/" . $pathBasename;

        return $pathFilename;

    }

    public function getOutputPath($path){

        $path   = pathinfo( $this->outputDir . "/" . $path);
        $pathDirname  = $path["dirname"];
        $pathBasename = $path["basename"];
        $pathFilename = $pathDirname . "/" . $pathBasename;

        return $pathFilename;

    }

} 