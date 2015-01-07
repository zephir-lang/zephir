<?php

namespace Zephir\Documentation\File;


use Zephir\ClassDefinition;

class SourceFile extends ClassFile {

    /**
     * @var ClassDefinition
     */
    protected $class;

    public function getTemplateName()
    {
        return "source.phtml";
    }

    public function getOutputFile()
    {
        return \Zephir\Documentation::sourceUrl($this->class);
    }

} 