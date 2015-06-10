<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

namespace Zephir\Documentation\File;

use Zephir\ClassDefinition;
use Zephir\Documentation\AbstractFile;

class Sitemap extends AbstractFile
{


    protected $classes;
    protected $namespaces;
    protected $baseUrl;

    public function __construct($baseUrl, $classList, $namespaceList)
    {
        $this->classes = $classList;
        $this->namespaces = $namespaceList;
        $this->baseUrl = rtrim($baseUrl, "/") . "/";
    }

    public function getTemplateName()
    {
        return ZEPHIRPATH . "/templates/Api/sitemap.php";
    }

    public function getData()
    {
        return array(
            "classes"    => $this->classes,
            "namespaces" => $this->namespaces,
            "baseUrl"    => $this->baseUrl,
        );
    }

    public function getOutputFile()
    {
        return "sitemap.xml";
    }
}
