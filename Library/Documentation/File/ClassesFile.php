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

namespace Zephir\Documentation\File;

use Zephir\ClassDefinition;
use Zephir\Documentation\AbstractFile;

class ClassesFile extends AbstractFile
{
    /**
     * @var ClassDefinition
     */
    protected $classes;

    public function __construct($config, $classList)
    {
        $this->classes = $classList;
    }

    public function getTemplateName()
    {
        return "classes.phtml";
    }

    public function getData()
    {
        return array(
            "classes" => $this->classes,
        );
    }

    public function getOutputFile()
    {
        return "classes.html";
    }
}
