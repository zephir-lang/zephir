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

namespace Zephir\Types;

/**
 * Central registry for type classification and operations.
 * Reduces repetitive switch/case statements throughout the codebase.
 */
final class TypeRegistry
{
    /**
     * Integer-like types
     */
    public const INTEGER_TYPES = ['int', 'uint', 'long', 'ulong'];

    /**
     * Character types
     */
    public const CHAR_TYPES = ['char', 'uchar'];

    /**
     * All numeric types (integers + chars + double)
     */
    public const NUMERIC_TYPES = ['int', 'uint', 'long', 'ulong', 'char', 'uchar', 'double'];

    /**
     * Scalar types
     */
    public const SCALAR_TYPES = ['int', 'uint', 'long', 'ulong', 'char', 'uchar', 'double', 'bool', 'string'];

    /**
     * Boolean types (aliases)
     */
    public const BOOLEAN_TYPES = ['bool', 'boolean'];

    /**
     * String types
     */
    public const STRING_TYPES = ['string', 'istring'];

    /**
     * Dynamic types
     */
    public const DYNAMIC_TYPES = ['variable', 'mixed', 'array', 'object'];

    /**
     * Check if a type is an integer type
     */
    public static function isInteger(string $type): bool
    {
        return in_array($type, self::INTEGER_TYPES, true);
    }

    /**
     * Check if a type is a character type
     */
    public static function isChar(string $type): bool
    {
        return in_array($type, self::CHAR_TYPES, true);
    }

    /**
     * Check if a type is numeric
     */
    public static function isNumeric(string $type): bool
    {
        return in_array($type, self::NUMERIC_TYPES, true);
    }

    /**
     * Check if a type is scalar
     */
    public static function isScalar(string $type): bool
    {
        return in_array($type, self::SCALAR_TYPES, true);
    }

    /**
     * Check if a type is boolean
     */
    public static function isBoolean(string $type): bool
    {
        return in_array($type, self::BOOLEAN_TYPES, true);
    }

    /**
     * Check if a type is string-like
     */
    public static function isString(string $type): bool
    {
        return in_array($type, self::STRING_TYPES, true);
    }

    /**
     * Check if a type is dynamic
     */
    public static function isDynamic(string $type): bool
    {
        return in_array($type, self::DYNAMIC_TYPES, true);
    }

    /**
     * Check if a type is integer-like or char
     */
    public static function isIntegerOrChar(string $type): bool
    {
        return self::isInteger($type) || self::isChar($type);
    }

    /**
     * Normalize boolean type aliases
     */
    public static function normalizeType(string $type): string
    {
        return match ($type) {
            'boolean' => 'bool',
            'integer' => 'int',
            default => $type,
        };
    }
}
