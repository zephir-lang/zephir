<?php

namespace Zephir\Documentation;

use Zephir\Config;
use Zephir\Exception;
use Zephir\CompilerFile;
use Zephir\ClassDefinition;
use Zephir\Documentation;

class Template
{
    protected $template;
    protected $data;
    protected $nestedLevel;
    protected $pathToRoot = "./";
    protected $themeOptions;
    protected $theme;
    /**
     * @var Config
     */
    protected $projectConfig;

    public function __construct(Theme $theme, $data, $template, $nestedLevel = 0)
    {


        // todo clean buffer before exception
        if ($nestedLevel > 800) {
            throw new Exception("Recursive inclusion detected in theme creation");
        }

        $this->theme = $theme;
        $this->data = $data;
        $this->template = $template;
        $this->nestedLevel = $nestedLevel;
    }

    /**
     * set a variable that will be accessible in the template
     * @param $name
     * @param $value
     */
    public function setVar($name, $value)
    {
        $this->data[$name] = $value;
    }

    /**
     * get a variable set with setVar()
     * @param $name
     * @return bool
     */
    public function getVar($name)
    {
        return isset($this->data[$name]) ? $this->data[$name] : null;
    }

    /**
     * find the value in the project configuration (e.g the version)
     * @param string $name the name of the config to get
     */
    public function projectConfig($name)
    {
        if (isset($this->projectConfig)) {
            return $this->projectConfig->get($name);
        } else {
            return null;
        }
    }

    /**
     * find the value of an option of the theme
     * @param string $name the name of the option to get
     */
    public function themeOption($name)
    {
        return isset($this->themeOptions[$name]) ? $this->themeOptions[$name] : null ;
    }

    /**
     * set the config of the project (it usually wraps the version, the theme config, etc...)
     * @param array $projectConfig
     */
    public function setProjectConfig($projectConfig)
    {
        $this->projectConfig = $projectConfig;
    }

    /**
     * add theme options to make them available during the render phase
     * @param array $themeOptions
     */
    public function setThemeOptions($themeOptions)
    {
        $this->themeOptions = $themeOptions;
    }

    /**
     * get a value from the theme config (theme.json file placed inside the theme directory)
     */
    public function getAssets()
    {
        $css = $this->theme->getThemeInfoExtendAware("css");
        $js = $this->theme->getThemeInfoExtendAware("javascript");

        return array(
            "css" => $css,
            "javascript"  => $js
        );
    }

    /**
     * the path to root for the hyperlink in the templates
     * @param string $pathToRoot
     */
    public function setPathToRoot($pathToRoot)
    {
        $this->pathToRoot = $pathToRoot;
    }

    /**
     * Generate an url relative to the current directory
     *
     * @param string $url the url we want to reach
     * @return string the relative path to the url
     */
    public function url($url)
    {
        if (is_string($url)) {
            if ($url{0} == "/") {
                return $this->getPathToRoot() . ltrim($url, "/");
            } elseif (is_string($url)) {
                return $url;
            }
        } elseif ($url instanceof ClassDefinition) {
            return $this->url(Documentation::classUrl($url));
        } elseif ($url instanceof CompilerFile) {
            return $this->url(Documentation::classUrl($url->getClassDefinition()));
        }

        return "";
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
        $path = $this->__getTemplatePath($this->template);
        ob_start();
        include($path);
        $content = ob_get_clean();

        return $content;
    }

    private function __getTemplatePath($fileName)
    {
        if ("/" == $fileName{0}) {
            return $fileName;
        }

        $inputFilename = $this->theme->getThemePathExtendsAware($fileName);

        if (!file_exists($inputFilename)) {
            throw new Exception("Template not found : $inputFilename");
        }

        return $inputFilename;
    }

    public function partial($fileName, array $data = array())
    {
        $newLevel = $this->nestedLevel+1;

        $template = new Template($this->theme, array_merge($this->data, $data), $fileName, $newLevel);
        $template->setPathToRoot($this->getPathToRoot());
        $template->setThemeOptions($this->themeOptions);
        $template->setProjectConfig($this->projectConfig);

        return $template->parse();
    }
}
