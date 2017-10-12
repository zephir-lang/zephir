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
use Zephir\CompilerFile;

class ClassFile extends AbstractFile
{
    /**
     * @var ClassDefinition
     */
    protected $class;

    /**
     *
     * @var CompilerFile
     */
    protected $compilerFile;

    public function __construct($config, CompilerFile $class)
    {
        $this->compilerFile = $class;
        $this->class = $class->getClassDefinition();
    }

    public function getTemplateName()
    {
        return "class.phtml";
    }

    public function getData()
    {
        $nsPieces = explode('\\', $this->class->getNamespace());

        $nsPatches = array();
        $nsStr = "";

        foreach ($nsPieces as $n) {
            if (strlen($nsStr) > 0) {
                $nsStr.= '\\';
            }
            $nsStr.= $n;
            $nsPatches[$n] = $nsStr;
        }

        return array(

            "classDefinition" => $this->class,
            "compilerFile"    => $this->compilerFile,
            "className" => $this->class->getName(),
            "classNamespace" => $this->class->getNamespace(),
            "fullName"  => $this->class->getCompleteName(),
            "methods"   => $this->class->getMethods(),
            "namespacePieces" => $nsPatches

        );
    }

    public function getOutputFile()
    {
        return \Zephir\Documentation::classUrl($this->class);
    }
}
