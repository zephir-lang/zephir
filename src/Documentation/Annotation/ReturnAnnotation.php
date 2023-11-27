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

/**
 * A return annotation that looks like  `(@)return type description`.
 */
class ReturnAnnotation extends Annotation
{
    protected string $description;
    protected string $returnType;

    public function getDescription(): string
    {
        if (!$this->contentParsed) {
            $this->parseContent();
        }

        return $this->description;
    }

    public function getReturnType(): string
    {
        if (!$this->contentParsed) {
            $this->parseContent();
        }

        return $this->returnType;
    }

    protected function parseContent(): void
    {
        $spaceIndex = strpos($this->string, ' ');

        $this->returnType = $this->string;

        if (false !== $spaceIndex) {
            $this->returnType  = substr($this->string, 0, $spaceIndex);
            $this->description = substr($this->string, $spaceIndex + 1);
        }

        $this->contentParsed = true;
    }
}
