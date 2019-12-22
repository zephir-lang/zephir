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
class Link extends Annotation
{
    /** @var string|null */
    protected $uri;

    /** @var string|null */
    protected $linkText;

    public function getUri()
    {
        if (!$this->contentParsed) {
            $this->parseContent();
        }

        return $this->uri;
    }

    public function getLinkText()
    {
        if (!$this->contentParsed) {
            $this->parseContent();
        }

        return $this->linkText;
    }

    protected function parseContent()
    {
        $spaceIndex = strpos($this->string, ' ');

        $this->uri = $this->string;

        if (false !== $spaceIndex) {
            $this->uri = substr($this->string, 0, $spaceIndex);
            $this->linkText = substr($this->string, $spaceIndex + 1);
        }

        $this->contentParsed = true;
    }
}
