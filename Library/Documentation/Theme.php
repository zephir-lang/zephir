<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Documentation;

use Zephir\Documentation;
use Zephir\Exception;

class Theme
{
    protected $themeDir;
    protected $outputDir;
    protected $themeConfig;
    protected $options;
    protected $projectConfig;
    /**
     * @var Documentation
     */
    protected $documentation;

    /**
     * @var Theme
     */
    protected $extendedTheme;

    protected $themeInfos = array();

    public function __construct($themeDir, $outputDir, $themeConfig, $config, Documentation $documentation)
    {
        $this->outputDir   = $outputDir;
        $this->themeConfig = $themeConfig;
        $this->themeDir    = $themeDir;
        $this->options     = $themeConfig["options"];
        $this->projectConfig= $config;
        $this->documentation = $documentation;


        $themeInfosPath = $this->getThemePath("theme.json");
        if ($themeInfosPath) {
            $themeInfos = json_decode(file_get_contents($themeInfosPath), true);
            if (!$themeInfos) {
                throw new Exception("Cant parse file $themeInfosPath");
            } else {
                $this->themeInfos = $themeInfos;
                if (isset($themeInfos["extends"])) {
                    $extThemePath = $documentation->findThemePathByName($themeInfos["extends"]);
                    if (!$extThemePath) {
                        throw new Exception("Unable to find extended theme " . $themeInfos["extends"]);
                    }

                    $this->extendedTheme = new Theme($extThemePath, $outputDir, $themeConfig, $config, $documentation);
                }
            }
        }
    }

    /**
     * parse and draw the specified file
     * @param AbstractFile $file
     */
    public function drawFile(AbstractFile $file)
    {
        $outputFile = ltrim($file->getOutputFile(), "/");

        $output   = pathinfo($this->outputDir . "/" . $outputFile);
        $outputDirname  = $output["dirname"];
        $outputBasename = $output["basename"];
        $outputFilename = $outputDirname . "/" . $outputBasename;

        // todo : check if writable
        if (!file_exists($outputDirname)) {
            mkdir($outputDirname, 0777, true);
        }

        $subDirNumber= count(explode("/", $outputFile))-1;

        if ($subDirNumber > 0) {
            $pathToRoot = str_repeat("../", $subDirNumber);
        } else {
            $pathToRoot = "./";
        }

        $template = new Template($this, $file->getData(), $file->getTemplateName());
        $template->setPathToRoot($pathToRoot);
        $template->setThemeOptions($this->options);
        $template->setProjectConfig($this->projectConfig);


        touch($outputFilename);
        $template->write($outputFilename);
    }

    /**
     * get assets from the theme info (theme.json file placed inside the theme directory)
     */
    public function getThemeInfo($name)
    {
        if (isset($this->themeInfos[$name])) {
            return $this->themeInfos[$name];
        }
        return null;
    }

    /**
     * similar with getThemeInfo but includes the value for all extended themes, and returns the results as an array
     * @return array
     */
    public function getThemeInfoExtendAware($name)
    {
        if ($this->extendedTheme) {
            $data = $this->extendedTheme->getThemeInfoExtendAware($name);
        } else {
            $data = array();
        }
        $info = $this->getThemeInfo($name);
        array_unshift($data, $info);

        return $data;
    }

    /**
     * copy the static directory of the theme into the output directory
     */
    public function buildStaticDirectory()
    {
        $outputStt = $this->getOutputPath("asset");

        if (!file_exists($outputStt)) {
            mkdir($outputStt, 0777, true);
        }

        if ($this->extendedTheme) {
            $this->extendedTheme->buildStaticDirectory();
        }

        $themeStt = $this->getThemePath("static");

        if ($themeStt) {
            $files = array();

            $this->__copyDir($themeStt, $outputStt . "/static", $files);

            foreach ($files as $f) {
                foreach ($this->options as $optName => $opt) {
                    $fcontent = file_get_contents($f);
                    $fcontent = str_replace("%_" . $optName . "_%", $opt, $fcontent);

                    file_put_contents($f, $fcontent);
                }
            }
        }
    }



    public function buildJsonClassDefinition($classList, NamespaceAccessor $nsA)
    {
        $output = array(

            "allClasses" => array(),
            "allNamespaces" => array(),
            "classes" => array(),
            "namespaces" => array()

        );

        foreach ($classList as $class) {
            $cDef = $class->getClassDefinition();
            $cName = $cDef->getCompleteName();

            $output["allClasses"][$cName] = array(
                "type" => $cDef->getType(),
                "name" => $cName,
                "shortname" => $cDef->getName()
            );

            if (!strpos($cName, "\\") > 0) {
                $output["classes"][] = $cName;
            }
        }


        $tree = $nsA->getNamespaceTree();
        $namespaces = $nsA->getByNamespace();


        foreach ($namespaces as $ns) {
            $subclasses     = array();
            $subnamespaces  = array();

            foreach ($ns->getClasses() as $scs) {
                $subclasses[] = $scs->getClassDefinition()->getCompleteName();
            }

            foreach ($ns->getNamespaces() as $sns) {
                $subnamespaces[] = $sns->getFullNamespace();
            }

            $output["allNamespaces"][$ns->getFullNamespace()] = array(

                "name"       => $ns->getFullNamespace(),
                "shortName"  => $ns->getShortName(),
                "parentName" => $ns->getParentName(),
                "classes"    => $subclasses,
                "namespaces" => $subnamespaces

            );

            if (!strpos($ns->getFullNamespace(), "\\") > 0) {
                $output["namespaces"][] = $ns->getFullNamespace();
            }
        }
        return json_encode($output);
    }

    public function createFile($path, $content)
    {
        $outputFile = $this->getOutputPath($path);
        touch($outputFile);
        file_put_contents($outputFile, $content);
    }

    private function __namespaceTreeHelper(NamespaceHelper $ns)
    {
        $output = array(
            "classes"    => array(),
            "namespaces" => array()
        );

        $subNs = $ns->getNamespaces();
        $subCs = $ns->getClasses();

        foreach ($subCs as $c) {
            $output["classes"][] = $c->getClassDefinition()->getCompleteName();
        }

        foreach ($subNs as $sns) {
            $output["namespaces"][$sns->getFullNamespace()] = $this->__namespaceTreeHelper($sns);
        }

        return $output;
    }

    /**
     * from : http://stackoverflow.com/questions/2050859/copy-entire-contents-of-a-directory-to-another-using-php
     * @param $src
     * @param $dst
     * @param $files
     */
    private function __copyDir($src, $dst, &$files = null)
    {
        $dir = opendir($src);
        @mkdir($dst);
        while (false !== ($file = readdir($dir))) {
            if (($file != '.') && ($file != '..')) {
                if (is_dir($src . '/' . $file)) {
                    $this->__copyDir($src . '/' . $file, $dst . '/' . $file, $files);
                } else {
                    copy($src . '/' . $file, $dst . '/' . $file);
                    if (is_array($files)) {
                        $files[] = $dst . '/' . $file;
                    }
                }
            }
        }
        closedir($dir);
    }

    /**
     * find the path to a file in the theme or from the extended theme
     * @param $path
     * @return string
     */
    public function getThemePathExtendsAware($path)
    {
        $newPath = $this->getThemePath($path);
        if (!$newPath) {
            if ($this->extendedTheme) {
                return $this->extendedTheme->getThemePathExtendsAware($path);
            }
        }
        return $newPath;
    }

    /**
     * find the path to a file in the theme
     * @param $path
     * @return string
     */
    public function getThemePath($path)
    {
        $path   = pathinfo($this->themeDir . "/" . $path);
        $pathDirname  = $path["dirname"];
        $pathBasename = $path["basename"];
        $pathFilename = $pathDirname . "/" . $pathBasename;

        if (!file_exists($pathFilename)) {
            return null;
        }

        return $pathFilename;
    }

    public function getOutputPath($path)
    {
        $path   = pathinfo($this->outputDir . "/" . $path);
        $pathDirname  = $path["dirname"];
        $pathBasename = $path["basename"];
        $pathFilename = $pathDirname . "/" . $pathBasename;

        return $pathFilename;
    }
}
