<?php

namespace Zephir\Documentation;

use Zephir\Exception;

use Zephir\CompilerFile;
use Zephir\ClassDefinition;
use Zephir\Documentation;

class Template
{

    protected $template;
    protected $rootDirectory;
    protected $data;
    protected $nestedLevel;
    protected $pathToRoot="./";
    protected $themeOptions;

    public function __construct($data, $rootDirectory, $template, $nestedLevel = 0)
    {

        // todo clean buffer before exception
        if ($nestedLevel>800) {
            throw new Exception("Recursive inclusion detected in theme creation");
        }
        
        // todo : securise parent inclusion
        $rootDirectory = rtrim($rootDirectory, "/");

        $this->data = $data;
        $this->rootDirectory = $rootDirectory;
        $this->template = $template;
        $this->nestedLevel = $nestedLevel;
    }

    public function themeOption($name)
    {
        return isset($this->themeOptions[$name]) ? $this->themeOptions[$name] : null ;
    }

    /**
     * add theme options to make them available during the render phase
     * @param type $themeOptions
     */
    public function setThemeOptions($themeOptions)
    {
        $this->themeOptions = $themeOptions;
    }
    
    /**
     * @param string $pathToRoot
     */
    public function setPathToRoot($pathToRoot)
    {
        $this->pathToRoot = $pathToRoot;
    }
    
    /**
     * Generate an url relative to the current directory
     * @param string $url the url we want to reach
     * @return the relative path to the url
     */
    public function url($url)
    {

        if (is_string($url)) {
            if ($url{0} == "/") {
                return $this->getPathToRoot() . ltrim($url, "/");
            } else if (is_string($url)) {
                return $url;
            }
        } else if ($url instanceof ClassDefinition) {
            return $this->url(Documentation::classUrl($url));
        } else if ($url instanceof CompilerFile) {
            return $this->url(Documentation::classUrl($url->getClassDefinition()));
        }
        
        return;
        
    }

    /**
     * @return string
     */
    public function getPathToRoot()
    {
        return $this->pathToRoot;
    }

    public function asset($name)
    {
        return $this->getPathToRoot() . "asset/" . rtrim($name);
    }

    public function write($outputFile)
    {
        $content = $this->parse();
        file_put_contents($outputFile, $content);
    }


    public function parse()
    {

        foreach ($this->data as $name => $value) {
            $$name = $value;
        }

        ob_start();
        include($this->__getSecureFilePath($this->template));
        $content = ob_get_clean();

        return $content;
    }

    private function __getSecureFilePath($fileName)
    {

        $input   = pathinfo($this->rootDirectory . "/" . $fileName);
        $inputDirname  = $input["dirname"];
        $inputBasename = $input["basename"];
        $inputFilename = $inputDirname . "/" . $inputBasename;

        if (!file_exists($inputFilename)) {
            throw new Exception("Template not found : $inputFilename");
        }

        return $inputFilename;
    }


    public function partial($fileName, array $data = array())
    {

        $newLevel = $this->nestedLevel+1;

        $template = new Template(array_merge($this->data, $data), $this->rootDirectory, $fileName, $newLevel);
        $template->setPathToRoot($this->getPathToRoot());
        $template->setThemeOptions($this->themeOptions);

        return $template->parse();
    }
}
