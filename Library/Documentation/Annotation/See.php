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
 * A link annotation that looks like  `(@)link uri text`
 *
 */
class See extends Annotation
{
    
    protected $resource;
    protected $text;


    protected function parseContent()
    {
        $spaceIndex = strpos($this->string, " ");
        
        if (false !== $spaceIndex) {
            $this->resource = substr($this->string, 0, $spaceIndex);
            $this->text = substr($this->string, $spaceIndex + 1);
        } else {
            $this->resource = $this->string;
        }
        
        $this->contentParsed = true;
    }
    
    
    public function getResource()
    {
        if (!$this->contentParsed) {
            $this->parseContent();
        }
        
        return $this->resource;
    }

    public function getText()
    {
        if (!$this->contentParsed) {
            $this->parseContent();
        }
        
        return $this->text;
    }
}
