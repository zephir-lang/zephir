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

namespace Zephir\Documentation\Annotation;

use Zephir\Documentation\Annotation;

use function strpos;
use function substr;

/**
 * A link annotation that looks like  `(@)link uri text`.
 */
class See extends Annotation
{
    protected $resource;
    protected $text;

    public function getResource(): string
    {
        if (!$this->contentParsed) {
            $this->parseContent();
        }

        return $this->resource;
    }

    public function getText(): string
    {
        if (!$this->contentParsed) {
            $this->parseContent();
        }

        return $this->text;
    }

    protected function parseContent(): void
    {
        $spaceIndex = strpos($this->string, ' ');

        $this->resource = $this->string;

        if (false !== $spaceIndex) {
            $this->resource = substr($this->string, 0, $spaceIndex);
            $this->text     = substr($this->string, $spaceIndex + 1);
        }

        $this->contentParsed = true;
    }
}
