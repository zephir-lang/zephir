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

        $nsPathes = array();
        $nsStr = "";

        foreach ($nsPieces as $n) {
            if (strlen($nsStr) > 0) {
                $nsStr.= '\\';
            }
            $nsStr.= $n;
            $nsPathes[$n] = $nsStr;
        }

        return array(

            "classDefinition" => $this->class,
            "compilerFile"    => $this->compilerFile,
            "className" => $this->class->getName(),
            "classNamespace" => $this->class->getNamespace(),
            "fullName"  => $this->class->getCompleteName(),
            "methods"   => $this->class->getMethods(),
            "namespacePieces" => $nsPathes

        );
    }

    public function getOutputFile()
    {
        return \Zephir\Documentation::classUrl($this->class);
    }
}
