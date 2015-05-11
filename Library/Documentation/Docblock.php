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

namespace Zephir\Documentation;

/**
 * Annotation
 *
 * A parsed Annotation
 */
class Docblock
{

    protected $description;

    /**
     *
     * @var Annotation[]
     */
    protected $annotations = array();

    protected $summary;

    public function getDescription()
    {
        return $this->description;
    }

    public function getAnnotations()
    {
        return $this->annotations;
    }

    /**
     *
     * @param string $type the annotation name you want to get
     * @return Annotation[] an array containing the annotations matching the name
     */
    public function getAnnotationcByType($type)
    {
        $annotations = array();
        foreach ($this->annotations as $an) {
            if ($an->getName() == $type) {
                $annotations[] = $an;
            }
        }
        return $annotations;
    }

    public function setDescription($description)
    {
        $this->description = $description;
    }

    public function addAnnotation($annotation)
    {
        $this->annotations[] = $annotation;
    }

    public function getSummary()
    {
        return $this->summary;
    }

    public function setSummary($summary)
    {
        $this->summary = $summary;
    }
}
