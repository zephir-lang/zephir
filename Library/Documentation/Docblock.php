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
class Docblock
{
    /**
     * @var string
     */
    protected $description;

    /**
     * @var Annotation[]
     */
    protected $annotations = array();

    /**
     * @var string
     */
    protected $summary;

    /**
     * @return string
     */
    public function getDescription()
    {
        return $this->description;
    }

    /**
     * @return Annotation[]
     */
    public function getAnnotations()
    {
        return $this->annotations;
    }

    /**
     * @param Annotation[] $annotations
     */
    public function setAnnotations(array $annotations)
    {
        $this->annotations = $annotations;
    }

    /**
     * @param string $type the annotation name you want to get
     * @return Annotation[] an array containing the annotations matching the name
     */
    public function getAnnotationsByType($type)
    {
        $annotations = array();
        foreach ($this->annotations as $an) {
            if ($an->getName() == $type) {
                $annotations[] = $an;
            }
        }
        return $annotations;
    }

    /**
     * @param string $description
     */
    public function setDescription($description)
    {
        $this->description = $description;
    }

    /**
     * @param Annotation $annotation
     */
    public function addAnnotation(Annotation $annotation)
    {
        $this->annotations[] = $annotation;
    }

    /**
     * @return string
     */
    public function getSummary()
    {
        return $this->summary;
    }

    /**
     * @param string $summary
     */
    public function setSummary($summary)
    {
        $this->summary = $summary;
    }

    /**
     * @return string
     */
    public function generate()
    {
        $docBlock = '**';

        if ($summary = $this->getSummary()) {
            $docBlock.= PHP_EOL . ' * ' . $summary;
        }

        if ($description = $this->getDescription()) {
            $docBlock.= PHP_EOL . ' *';
            $docBlock.= PHP_EOL . ' *';

            foreach ((array)explode("\n", $description) as $line) {
                $docBlock.= PHP_EOL . ' * ' . trim($line);
            }

            $docBlock.= PHP_EOL . ' *';
        }

        if ($annotations = $this->getAnnotations()) {
            foreach ($annotations as $annotation) {
                $docBlock.= PHP_EOL . ' * @' . $annotation->getName() . ' ' . $annotation->getString();
            }
        }

        return $docBlock . PHP_EOL . ' *';
    }

    /**
     * @return string
     */
    public function __toString()
    {
        return $this->generate();
    }
}
