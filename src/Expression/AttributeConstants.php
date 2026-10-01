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

namespace Zephir\Expression;

use function strtolower;

/**
 * Maps the `Attribute::*` class constants to the engine macros that define
 * them (issue #2738).
 *
 * Their values are version-shaped: PHP 8.5 added `TARGET_CONSTANT` and
 * renumbered `TARGET_ALL` and `IS_REPEATABLE`. A value read from the PHP that
 * runs `zephir generate` is therefore wrong wherever the C is compiled against
 * a different PHP, so the macro is emitted instead, exactly as php-src's own
 * `zend_attributes_arginfo.h` does.
 *
 * `ZEND_ATTRIBUTE_TARGET_CONST` exists only on PHP 8.5+. Compiled against an
 * older PHP it is an undeclared identifier, which is the closest a build can
 * get to PHP's own "Undefined constant Attribute::TARGET_CONSTANT".
 */
final class AttributeConstants
{
    /**
     * Keyed by the PHP constant name, which is case-sensitive. Two names differ
     * from their macro: `TARGET_CLASS_CONSTANT` and `TARGET_CONSTANT`.
     */
    private const MACROS = [
        'TARGET_CLASS'          => 'ZEND_ATTRIBUTE_TARGET_CLASS',
        'TARGET_FUNCTION'       => 'ZEND_ATTRIBUTE_TARGET_FUNCTION',
        'TARGET_METHOD'         => 'ZEND_ATTRIBUTE_TARGET_METHOD',
        'TARGET_PROPERTY'       => 'ZEND_ATTRIBUTE_TARGET_PROPERTY',
        'TARGET_CLASS_CONSTANT' => 'ZEND_ATTRIBUTE_TARGET_CLASS_CONST',
        'TARGET_PARAMETER'      => 'ZEND_ATTRIBUTE_TARGET_PARAMETER',
        'TARGET_CONSTANT'       => 'ZEND_ATTRIBUTE_TARGET_CONST',
        'TARGET_ALL'            => 'ZEND_ATTRIBUTE_TARGET_ALL',
        'IS_REPEATABLE'         => 'ZEND_ATTRIBUTE_IS_REPEATABLE',
    ];

    /**
     * @param string $className resolved class name, without a leading `\`
     *
     * @return string|null the C macro, or null when this is not an `Attribute` flag
     */
    public static function macro(string $className, string $constant): ?string
    {
        // PHP class names are case-insensitive.
        if ('attribute' !== strtolower($className)) {
            return null;
        }

        return self::MACROS[$constant] ?? null;
    }
}
