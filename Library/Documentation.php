<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;
use Zephir\Documentation\File;
use Zephir\Documentation\Theme;


/**
 * Documentation Generator
 */
class Documentation
{

    /**
     * @var Config
     */
    protected $config;

    /**
     * @var CompilerFile[]
     */
    protected $classes;

    /**
     * @var Theme
     */
    protected $theme;

    /**
     * @param CompilerFile[] $files
     * @param Config         $config
     */
    public function __construct(array $classes,Config $config)
    {
        $this->config = $config;
        $this->classes = $classes;

        $themeConfig = $config->get("theme","documentation");


        if(!$themeConfig)
            throw new ConfigException("Theme configuration is not present");

        $themeDir = realpath(ZEPHIRPATH . "/templates/Documentation/themes/" . $themeConfig["name"]);

        if(!file_exists($themeDir))
            throw new ConfigException("There is no theme name " . $themeConfig["name"]);


        $outputDir = $config->get("output-directory","documentation");
        $outputDir = str_replace('%version%', $this->config->get('version'), $outputDir);



        if(!$outputDir)
            throw new ConfigException("Documentation's output directory is not configured");

        if(!file_exists($outputDir))
            if(!mkdir($outputDir,0777,true))
                throw new Exception("Can't write output directory $outputDir");

        if(!is_writable($outputDir))
            throw new Exception("Can't write output directory $outputDir");



        $this->theme = new Theme($themeDir,$outputDir,$themeConfig);


    }

    public function build(){

        $files = array();

        foreach($this->classes as $className => $class){
            $file = new File\ClassFile($this->config,$class->getClassDefinition());

            $this->theme->drawFile($file);

        }

    }




    protected function __getThemeDirectory(){
        return $this->config->get("directory","theme");
    }

    protected function __getOutputDirectory(){
        return $this->config->get("output-directory");
    }

}