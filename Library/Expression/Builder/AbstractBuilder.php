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
namespace Zephir\Expression\Builder;

/**
 * Class AbstractBuilder
 * @package Zephir\Builder
 */
abstract class AbstractBuilder
{
    private $file = null;
    private $line = null;
    private $char = null;

    /**
     * @param string $file
     * @param int $line
     * @param null $char
     * @return $this
     */
    public function setParseInfo($file, $line, $char = null)
    {
        $this->setFile($file);
        $this->setLine($line);

        if ($char !== null) {
            $this->setChar($char);
        }

        return $this;
    }

    /**
     * @param string $file
     * @return AbstractBuilder
     */
    public function setFile($file)
    {
        $this->file = $file;
        return $this;
    }

    /**
     * @return string
     */
    public function getFile()
    {
        return $this->file;
    }

    /**
     * @param string $line
     * @return AbstractBuilder
     */
    public function setLine($line)
    {
        $this->line = $line;
        return $this;
    }

    /**
     * @return string
     */
    public function getLine()
    {
        return $this->line;
    }

    /**
     * @param int $char
     * @return AbstractBuilder
     */
    public function setChar($char)
    {
        $this->char = $char;
        return $this;
    }

    /**
     * @return int
     */
    public function getChar()
    {
        return $this->char;
    }

    /**
     * @return array
     *
     * @deprecated since version 0.8.0, to be removed in 1.0.
     *             Use {@link \Zephir\Builder\AbstractBuilder::build()}
     */
    public function get()
    {
        return $this->build();
    }

    /**
     * @return array
     */
    public function build()
    {
        $expression = $this->preBuild();

        if (!isset($expression['file']) && (($file = $this->getFile()) && $file !== null)) {
            $expression['file'] = $file;
        }

        if (!isset($expression['line']) && (($line = $this->getLine()) && $line !== null)) {
            $expression['line'] = $line;
        }

        if (!isset($expression['char']) && (($char = $this->getChar()) && $char !== null)) {
            $expression['char'] = $char;
        }

        return $this->resolve($expression);
    }

    /**
     * @return array
     */
    abstract protected function preBuild();

    /**
     * @param $expr
     * @return array
     */
    protected function resolve($expr)
    {
        if (is_array($expr)) {
            foreach ($expr as &$value) {
                if (!is_scalar($value)) {
                    $value = $this->resolve($value);
                }
            }

            return $expr;
        }

        if (is_object($expr) && $expr instanceof AbstractBuilder) {
            return $expr->build();
        }

        return $expr;
    }
}
