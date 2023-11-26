<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder;

abstract class AbstractBuilder
{
    private $file;
    private $line;
    private $char;

    /**
     * @param string $file
     * @param int    $line
     * @param null   $char
     *
     * @return $this
     */
    public function setParseInfo($file, $line, $char = null)
    {
        $this->setFile($file);
        $this->setLine($line);

        if (null !== $char) {
            $this->setChar($char);
        }

        return $this;
    }

    /**
     * @param string $file
     *
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
     *
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
     *
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

        if (!isset($expression['file']) && (($file = $this->getFile()) && null !== $file)) {
            $expression['file'] = $file;
        }

        if (!isset($expression['line']) && (($line = $this->getLine()) && null !== $line)) {
            $expression['line'] = $line;
        }

        if (!isset($expression['char']) && (($char = $this->getChar()) && null !== $char)) {
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
     *
     * @return array
     */
    protected function resolve($expr)
    {
        if (\is_array($expr)) {
            foreach ($expr as &$value) {
                if (!is_scalar($value)) {
                    $value = $this->resolve($value);
                }
            }

            return $expr;
        }

        if (\is_object($expr) && $expr instanceof self) {
            return $expr->build();
        }

        return $expr;
    }
}
