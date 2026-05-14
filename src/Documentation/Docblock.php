<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Documentation;

use function explode;
use function trim;

use const PHP_EOL;

/**
 * A parsed Annotation
 */
class Docblock
{
    /**
     * @var Annotation[]
     */
    protected array $annotations = [];

    protected string $description;

    protected string $summary;

    public function __toString()
    {
        return $this->generate();
    }

    public function addAnnotation(Annotation $annotation): void
    {
        $this->annotations[] = $annotation;
    }

    public function generate(): string
    {
        $docBlock         = '**';
        $summaryBlock     = $this->getSummary();
        $descriptionBlock = $this->getDescription();
        $annotationsBlock = $this->getAnnotations();

        if ($summaryBlock) {
            $docBlock .= PHP_EOL . ' * ' . $summaryBlock;
        }

        if ($descriptionBlock) {
            $docBlock .= PHP_EOL . ' *';
            $docBlock .= PHP_EOL . ' *';

            foreach (explode("\n", $descriptionBlock) as $line) {
                $docBlock .= PHP_EOL . ' * ' . trim($line);
            }

            $docBlock .= PHP_EOL . ' *';
        }

        if ($annotationsBlock) {
            foreach ($annotationsBlock as $annotation) {
                $docBlock .= PHP_EOL . ' * @' . $annotation->getName() . ' ' . $annotation->getString();
            }
        }

        return $docBlock . PHP_EOL . ' *';
    }

    /**
     * @return Annotation[]
     */
    public function getAnnotations(): array
    {
        return $this->annotations;
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

    public function getDescription(): string
    {
        return $this->description;
    }

    public function getSummary(): string
    {
        return $this->summary;
    }

    /**
     * @param Annotation[] $annotations
     */
    public function setAnnotations(array $annotations): void
    {
        $this->annotations = $annotations;
    }

    public function setDescription(string $description): void
    {
        $this->description = $description;
    }

    public function setSummary(string $summary): void
    {
        $this->summary = $summary;
    }
}
