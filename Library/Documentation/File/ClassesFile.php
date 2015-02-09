<?php

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
        return "classes.html" ;
    }
}
