<?php

namespace Zephir\Documentation;


use Zephir\ClassDefinition;
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

    public function buildJsonClassDefinition($classList,NamespaceAccessor $nsA){

        $output = array(

            "allClasses" => array(),
            "allNamespaces" => array(),
            "classes" => array(),
            "namespaces" => array()


        );

        foreach($classList as $class){

            $cDef = $class->getClassDefinition();
            $cName = $cDef->getCompleteName();

            $output["allClasses"][$cName] = array(
                "type" => $cDef->getType(),
                "name" => $cName,
                "shortname" => $cDef->getName()
            );

            if( ! strpos($cName,"\\") > 0 ){
                $output["classes"][] = $cName;
            }


        }


        $tree = $nsA->getNamespaceTree();
        $namespaces = $nsA->getByNamespace();


        foreach($namespaces as $ns){

            $subclasses     = array();
            $subnamespaces  = array();

            foreach($ns->getClasses() as $scs){
                $subclasses[] = $scs->getClassDefinition()->getCompleteName();
            }

            foreach($ns->getNamespaces() as $sns){
                $subnamespaces[] = $sns->getFullNamespace();
            }

            $output["allNamespaces"][$ns->getFullNamespace()] = array(

                "name"       => $ns->getFullNamespace(),
                "shortName"  => $ns->getShortName(),
                "parentName" => $ns->getParentName(),
                "classes"    => $subclasses,
                "namespaces" => $subnamespaces

            );

            if( ! strpos($ns->getFullNamespace(),"\\") > 0 ){
                $output["namespaces"][] = $ns->getFullNamespace();
            }

        }



        return json_encode($output);
    }

    public function createFile($path,$content){
        $outputFile = $this->getOutputPath($path);
        touch($outputFile);
        file_put_contents($outputFile,$content);
    }

    private function __namespaceTreeHelper(NamespaceHelper $ns){

        $output = array(
            "classes"    => array(),
            "namespaces" => array()
        );

        $subNs = $ns->getNamespaces();
        $subCs = $ns->getClasses();

        foreach($subCs as $c){
            $output["classes"][] = $c->getClassDefinition()->getCompleteName();
        }

        foreach($subNs as $sns){
            $output["namespaces"][$sns->getFullNamespace()] = $this->__namespaceTreeHelper($sns);
        }

        return $output;

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