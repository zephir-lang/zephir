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

namespace Zephir\Documentation;

/**
 * Annotation
 *
 * A parsed Annotation
 */
class Annotation
{
    protected $name;

    protected $string;

    protected $contentParsed = false;

    public function __construct($name, $string)
    {
        $this->name = $name;
        $this->string = trim($string);
    }

    public function getString()
    {
        return $this->string;
    }

    public function setString($string)
    {
        $this->string = $string;
    }

    public function getName()
    {
        return $this->name;
    }
}
