<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

final class Types
{
    const T_INT = 'int';
    const T_UINT = 'uint';
    const T_CHAR = 'char';
    const T_UCHAR = 'uchar';
    const T_LONG = 'long';
    const T_ULONG = 'ulong';
    const T_DOUBLE = 'double';
    const T_FLOAT = 'float';
    const T_NUMBER = 'number';
    const T_NULL = 'null';
    const T_BOOL = 'bool';
    const T_STRING = 'string';
    const T_ISTRING = 'istring';
    const T_VARIABLE = 'variable';
    const T_MIXED = 'mixed';
    const T_ARRAY = 'array';
    const T_VOID = 'void';
    const T_OBJECT = 'object';
    const T_CALLABLE = 'callable';
    const T_RESOURCE = 'resource';
    const T_ITERABLE = 'iterable';
    const T_UNDEFINED = 'undefined';

    public static function getTypesBySpecification(?array $returnTypes): string
    {
        if (!static::hasReturnType($returnTypes)) {
            return '';
        }

        return static::getCompatibleReturnType($returnTypes);
    }

    public static function isVoid(array $returnTypes): bool
    {
        return \in_array(static::T_VOID, $returnTypes, true);
    }

    public static function getCompatibleReturnType(array $types): string
    {
        foreach ($types as $k => $type) {
            $isInteger = static::isTypeIntegerCompatible($type);
            $isDouble = static::isTypeDoubleCompatible($type);

            $typeInteger = isset($typeInteger) ? ($isInteger && $typeInteger) : $isInteger;
            $typeDouble = isset($typeDouble) ? ($isDouble && $typeDouble) : $isDouble;
        }

        if ($typeInteger) {
            $compatibleType = static::T_INT;
        } elseif ($typeDouble) {
            $compatibleType = static::T_FLOAT;
        }

        return $compatibleType ?? static::T_MIXED;
    }

    private static function isTypeIntegerCompatible(string $type): bool
    {
        switch ($type) {
            case static::T_INT:
            case static::T_UINT:
            case static::T_CHAR:
            case static::T_UCHAR:
            case static::T_LONG:
            case static::T_ULONG:
                return true;
            default:
                return false;
        }
    }

    private static function isTypeDoubleCompatible(string $type): bool
    {
        return $type === static::T_DOUBLE;
    }

    /**
     * Checks if the parsed returns has return-type or cast hints.
     *
     * @param array|null $returnTypes
     *
     * @return bool
     */
    private static function hasReturnType(?array $returnTypes): bool
    {
        if (null === $returnTypes) {
            return false;
        }

        if (0 === \count($returnTypes)) {
            return false;
        }

        return true;
    }
}
