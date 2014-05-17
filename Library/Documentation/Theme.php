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



        $subDirNumber= count(explode("/", $file->getOutputFile()))-1;

        if($subDirNumber > 0)
            $pathToRoot = str_repeat("../",$subDirNumber);
        else
            $pathToRoot = "./";

        $template = new Template($file->getData() , $this->themeDir , $file->getTemplateName());
        $template->setPathToRoot($pathToRoot);

        touch($outputFilename);
        $template->write($outputFilename);

        $this->buildStaticDirectory();

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