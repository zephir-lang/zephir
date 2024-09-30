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

use function strlen;
use function trim;

use const PHP_EOL;

/**
 * Helper to parse raw docblocks to structured object.
 */
class DocblockParser
{
    protected $annotation;
    protected $annotationLen;
    protected $annotationNameOpen;
    protected $annotationOpen;
    protected $commentOpen;

    // Parsing helpers
    protected $currentAnnotationContentStr;
    protected $currentAnnotationStr;
    protected $currentChar;
    protected $currentCharIndex;
    protected $currentLine;
    protected $descriptionOpen;
    protected $descriptionStr;

    // parsed data
    /**
     * @var Docblock
     */
    protected $docblockObj;
    protected $ignoreSpaces;
    protected $ignoreStar;
    protected $summaryOpen;
    protected $summaryStr;

    /**
     */
    public function __construct(string $annotation)
    {
        $this->annotation = $annotation;
    }

    /**
     * return the parsed docblock. It will return null until you run the parse() method.
     *
     * @return Docblock the parsed docblock
     */
    public function getParsedDocblock()
    {
        return $this->docblockObj;
    }

    /**
     * Parses the internal annotation string.
     *
     * @return Docblock the parsed docblock
     */
    public function parse()
    {
        $this->docblockObj = new Docblock();

        $this->ignoreSpaces       = false;
        $this->ignoreStar         = true;
        $this->commentOpen        = false;
        $this->annotationNameOpen = false;
        $this->annotationOpen     = false;
        $this->summaryOpen        = true;
        $this->descriptionOpen    = false;

        $this->currentAnnotationStr        = null;
        $this->currentAnnotationContentStr = null;

        $this->summaryStr     = '';
        $this->descriptionStr = '';

        $this->currentLine      = 0;
        $this->currentCharIndex = 0;
        $this->annotationLen    = strlen($this->annotation);
        $this->currentChar      = $this->annotation[0];

        while (null !== $this->currentChar) {
            $currentChar = $this->currentChar;

            if ($this->ignoreSpaces && ctype_space($currentChar)) {
            } else {
                if (!$this->commentOpen) {
                    if ('/' == $currentChar) {
                        if ('*' == $this->nextCharacter() && '*' == $this->nextCharacter()) {
                            $this->commentOpen = true;
                        } else {
                            continue;
                        }
                    }
                } else {
                    if ('*' == $currentChar) {
                        $nextCharacter = $this->nextCharacter();
                        if ('/' == $nextCharacter) {
                            // stop annotation parsing on end of comment
                            $this->__tryRegisterAnnotation();
                            break;
                        } elseif ($this->ignoreStar) {
                            if (' ' == $nextCharacter) {
                                $this->nextCharacter();
                            }
                            continue;
                        }
                    }

                    if ('@' == $currentChar) {
                        $this->descriptionStr = trim($this->descriptionStr);
                        if ($this->descriptionOpen && $this->descriptionStr !== '') {
                            $this->descriptionOpen = false;
                        }

                        $this->currentAnnotationStr        = '';
                        $this->currentAnnotationContentStr = '';

                        $this->ignoreSpaces       = false;
                        $this->annotationNameOpen = true;
                    } elseif ($this->annotationNameOpen || $this->annotationOpen) {
                        // stop annotation parsing on new line
                        if ("\n" == $currentChar || "\r" == $currentChar) {
                            $this->__tryRegisterAnnotation();

                            $this->ignoreSpaces = false;
                            $this->ignoreStar   = true;
                        } elseif ($this->annotationNameOpen) {
                            if (ctype_space($currentChar)) {
                                $this->annotationNameOpen = false;
                                $this->annotationOpen     = true;
                            } else {
                                $this->currentAnnotationStr .= $currentChar;
                            }
                        } elseif ($this->annotationOpen) {
                            $this->currentAnnotationContentStr .= $currentChar;
                        }
                    } elseif ($this->summaryOpen) {
                        // stop summary on new line
                        if ($this->summaryStr !== '' && ("\n" == $currentChar || "\r" == $currentChar)) {
                            $this->summaryOpen     = false;
                            $this->descriptionOpen = true;
                            $this->ignoreStar      = true;
                        } else {
                            $this->summaryStr .= $currentChar;
                        }
                    } elseif ($this->descriptionOpen) {
                        // stop description on new line
                        if ("\n" == $currentChar || "\r" == $currentChar) {
                            $this->descriptionStr .= PHP_EOL;
                        } else {
                            $this->descriptionStr .= $currentChar;
                        }
                    }
                }
            }

            $this->nextCharacter();
        }

        $this->docblockObj->setSummary(trim($this->summaryStr));
        $this->docblockObj->setDescription(trim($this->descriptionStr));

        return $this->docblockObj;
    }

    /**
     * @param string $name   the annotation name
     * @param string $string the annotation name
     *
     * @return Annotation
     */
    private function __createAnnotation($name, $string)
    {
        switch ($name) {
            case 'link':
                $annotation = new Annotation\Link($name, $string);
                $annotation->getLinkText();
                break;

            case 'return':
                $annotation = new Annotation\ReturnAnnotation($name, $string);
                $annotation->getReturnType();
                break;

            case 'see':
                $annotation = new Annotation\See($name, $string);
                $annotation->getResource();
                break;

            default:
                $annotation = new Annotation($name, $string);
                break;
        }

        return $annotation;
    }

    /**
     * check if there is a currently parsed annotation, registers it, and stops the current annotation parsing.
     */
    private function __tryRegisterAnnotation(): void
    {
        if (($this->annotationNameOpen || $this->annotationOpen) && $this->currentAnnotationStr !== '') {
            $annotation = $this->__createAnnotation($this->currentAnnotationStr, $this->currentAnnotationContentStr);
            $this->docblockObj->addAnnotation($annotation);
        }

        $this->annotationNameOpen = false;
        $this->annotationOpen     = false;
    }

    /**
     * moves the current cursor to the next character.
     *
     * @return string the new current character
     */
    private function nextCharacter()
    {
        ++$this->currentCharIndex;

        if ($this->annotationLen <= $this->currentCharIndex) {
            $this->currentChar = null;
        } else {
            $this->currentChar = $this->annotation[$this->currentCharIndex];
        }

        return $this->currentChar;
    }
}
