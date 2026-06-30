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

namespace Zephir\Zept;

use RuntimeException;

/**
 * Thrown when a `.zept` file is structurally malformed (missing or conflicting
 * sections). The message always names the offending file so failures point at
 * the source, not the parser.
 */
final class ZeptParseException extends RuntimeException
{
    public static function in(string $path, string $problem): self
    {
        return new self(sprintf('Malformed .zept file "%s": %s', $path, $problem));
    }
}
