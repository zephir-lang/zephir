<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation;

/**
 * Annotation.
 *
 * A parsed Annotation
 */
class Annotation
{
    protected $name;

    protected $string;

    protected $contentParsed = false;

    public function __construct(string $name, string $string)
    {
        $this->name = $name;
        $this->string = trim($string);
    }

    public function getString(): string
    {
        return $this->string;
    }

    public function setString(string $string)
    {
        $this->string = $string;
    }

    public function getName(): string
    {
        return $this->name;
    }
}
