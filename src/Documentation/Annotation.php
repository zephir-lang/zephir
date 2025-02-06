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

use function trim;

/**
 * A parsed Annotation
 */
class Annotation
{
    protected bool $contentParsed = false;

    public function __construct(protected string $name, protected string $string)
    {
        $this->string = trim($string);
    }

    public function getName(): string
    {
        return $this->name;
    }

    public function getString(): string
    {
        return $this->string;
    }

    public function setString(string $string): void
    {
        $this->string = $string;
    }
}
