<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation;

/**
 * A parsed Annotation
 */
class Docblock
{
    /**
     * @var string
     */
    protected string $description;

    /**
     * @var Annotation[]
     */
    protected array $annotations = [];

    /**
     * @var string
     */
    protected string $summary;

    /**
     * @return string
     */
    public function __toString()
    {
        return $this->generate();
    }

    /**
     * @return string
     */
    public function getDescription(): string
    {
        return $this->description;
    }

    /**
     * @return Annotation[]
     */
    public function getAnnotations(): array
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
     *
     * @return Annotation[] an array containing the annotations matching the name
     */
    public function getAnnotationsByType(string $type): array
    {
        $annotation = [];

        foreach ($this->annotations as $an) {
            if ($an->getName() === $type) {
                $annotation[] = $an;
            }
        }

        return $annotation;
    }

    /**
     * @param string $description
     */
    public function setDescription(string $description)
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
    public function getSummary(): string
    {
        return $this->summary;
    }

    /**
     * @param string $summary
     */
    public function setSummary(string $summary): void
    {
        $this->summary = $summary;
    }

    /**
     * @return string
     */
    public function generate(): string
    {
        $docBlock = '**';
        $summaryBlock = $this->getSummary();
        $descriptionBlock = $this->getDescription();
        $annotationsBlock = $this->getAnnotations();

        if ($summaryBlock) {
            $docBlock .= PHP_EOL.' * '.$summaryBlock;
        }

        if ($descriptionBlock) {
            $docBlock .= PHP_EOL.' *';
            $docBlock .= PHP_EOL.' *';

            foreach (explode("\n", $descriptionBlock) as $line) {
                $docBlock .= PHP_EOL.' * '.trim($line);
            }

            $docBlock .= PHP_EOL.' *';
        }

        if ($annotationsBlock) {
            foreach ($annotationsBlock as $annotation) {
                $docBlock .= PHP_EOL.' * @'.$annotation->getName().' '.$annotation->getString();
            }
        }

        return $docBlock.PHP_EOL.' *';
    }
}
