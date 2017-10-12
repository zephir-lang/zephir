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

class SourceFile extends ClassFile
{
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
