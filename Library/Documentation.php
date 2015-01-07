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
    
    protected $logger;

    /**
     * @param CompilerFile[] $files
     * @param Config         $config
     */
    public function __construct(array $classes,Config $config,  \Zephir\Logger $logger)
    {
        
        ksort($classes);
        
        $this->config = $config;
        $this->classes = $classes;
        $this->logger  = $logger;

        $themeConfig = $config->get("theme","api");
        
        if(!$themeConfig)
            throw new ConfigException("Theme configuration is not present");
        
        if( !isset($themeConfig["name"]) || !$themeConfig["name"])
            throw new ConfigException("There is no theme ");
        
        $themeDir = realpath(ZEPHIRPATH . "/templates/Api/themes/" . $themeConfig["name"]);

        if(!file_exists($themeDir))
            throw new ConfigException("There is no theme name " . $themeConfig["name"]);

        $outputDir = $this->config->get('path', 'api');
        $outputDir = str_replace('%version%', $this->config->get('version'), $outputDir);

        if(!$outputDir)
            throw new ConfigException("Api path is not configured");

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
            $file = new File\ClassFile($this->config,$class);
            $this->theme->drawFile($file);
            
            $sfile = new File\SourceFile($this->config,$class);
            $this->theme->drawFile($sfile);
            
        }

        $file = new File\ClassesFile($this->config,$this->classes);
        $this->theme->drawFile($file);
        
        $this->theme->buildStaticDirectory();
        
    }
    
    public static function classUrl(ClassDefinition $c){
        return "/class/" . str_replace("\\","/", $c->getCompleteName()) . ".html";
    }
    
    public static function namespaceUrl($ns){
        return "/namespace/" . str_replace("\\","/", $ns) . ".html";
    }
    
    public static function sourceUrl(ClassDefinition $c){
        return "/source/" . str_replace("\\","/", $c->getCompleteName()) . ".html";
    }

}