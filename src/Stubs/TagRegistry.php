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

namespace Zephir\Stubs;

use function in_array;
use function ltrim;
use function preg_match;
use function str_starts_with;

/**
 * Registry of docblock tags that the stubs generator preserves verbatim
 * from .zep source into generated .php stubs.
 *
 * Preserved tags are NOT parsed for type/name/description and do NOT
 * participate in auto-generated @return / @param suppression.
 */
final class TagRegistry
{
    /**
     * Tag names (without leading @) that are preserved verbatim.
     */
    public const PRESERVED_EXACT = [
        'extends',
        'implements',
        'template',
        'template-contravariant',
        'template-covariant',
    ];

    /**
     * Tag prefixes (without leading @, with trailing -). Any tag starting
     * with one of these prefixes is preserved verbatim.
     */
    public const PRESERVED_PREFIXES = [
        'phpstan-',
        'psalm-',
    ];

    /**
     * @param string $tagName Tag name without the leading @
     *
     * @return bool
     */
    public static function isPreservedTag(string $tagName): bool
    {
        if ('' === $tagName) {
            return false;
        }

        if (in_array($tagName, self::PRESERVED_EXACT, true)) {
            return true;
        }

        foreach (self::PRESERVED_PREFIXES as $prefix) {
            if (str_starts_with($tagName, $prefix) && $tagName !== $prefix) {
                return true;
            }
        }

        return false;
    }

    /**
     * @param string $line A docblock line (with or without leading whitespace
     *                     and asterisks already stripped by DocBlock parsing).
     *
     * @return bool
     */
    public static function isPreservedLine(string $line): bool
    {
        $trimmed = ltrim($line);

        // Fast path: skip regex for non-tag lines.
        if ('' === $trimmed || $trimmed[0] !== '@') {
            return false;
        }

        if (1 !== preg_match('/^@([a-zA-Z][a-zA-Z0-9_-]*)/', $trimmed, $matches)) {
            return false;
        }

        return self::isPreservedTag($matches[1]);
    }
}
