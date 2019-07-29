<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation\Annotation;

use Zephir\Documentation\Annotation;

/**
 * A link annotation that looks like  `(@)link uri text`.
 */
class See extends Annotation
{
    protected $resource;
    protected $text;

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

    protected function parseContent()
    {
        $spaceIndex = strpos($this->string, ' ');

        if (false !== $spaceIndex) {
            $this->resource = substr($this->string, 0, $spaceIndex);
            $this->text = substr($this->string, $spaceIndex + 1);
        } else {
            $this->resource = $this->string;
        }

        $this->contentParsed = true;
    }
}
