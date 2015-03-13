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

namespace Zephir\Documentation\Annotation;

use Zephir\Documentation\Annotation;
use Zephir\Documentation\Docblock;

/**
 * A return annotation that looks like  `(@)return type description`
 */
class ReturnAnnotation extends Annotation
{
    
    protected $returnType;
    protected $description;


    protected function parseContent()
    {
        $spaceIndex = strpos($this->string, " ");
        
        if (false !== $spaceIndex) {
            $this->returnType = substr($this->string, 0, $spaceIndex);
            $this->description = substr($this->string, $spaceIndex + 1);
        } else {
            $this->returnType = $this->string;
        }
        
        $this->contentParsed = true;
    }
    
    public function getReturnType()
    {
        if (!$this->contentParsed) {
            $this->parseContent();
        }
        
        return $this->returnType;
    }

    public function getDescription()
    {
        if (!$this->contentParsed) {
            $this->parseContent();
        }
        
        return $this->description;
    }
}
