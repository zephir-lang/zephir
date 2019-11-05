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
            $isBoolean = static::isTypeBoolCompatible($type);
            $isArray = static::isTypeArrayCompatible($type);
            $isNull = static::isTypeNullCompatible($type);
            $isSpecial = static::isTypeSpecialCompatible($type);
            $isVoid = static::isTypeVoidCompatible($type);
            $isObject = static::isTypeObjectCompatible($type);

            $typeInteger = isset($typeInteger) ? ($isInteger && $typeInteger) : $isInteger;
            $typeDouble = isset($typeDouble) ? ($isDouble && $typeDouble) : $isDouble;
            $typeBoolean = isset($typeBoolean) ? ($isBoolean && $typeBoolean) : $isBoolean;
            $typeArray = isset($typeArray) ? ($isArray && $typeArray) : $isArray;
            $typeNull = isset($typeNull) ? ($isNull && $typeNull) : $isNull;
            $typeSpecial = isset($typeSpecial) ? ($isSpecial && $typeSpecial) : $isSpecial;
            $typeVoid = isset($typeVoid) ? ($isVoid && $typeVoid) : $isVoid;
            $typeObject = isset($typeObject) ? ($isObject && $typeObject) : $isObject;
        }

        if ($typeInteger) {
            $compatibleType = static::T_INT;
        } elseif ($typeDouble) {
            $compatibleType = static::T_FLOAT;
        } elseif ($typeBoolean) {
            $compatibleType = static::T_BOOL;
        } elseif ($typeArray) {
            $compatibleType = static::T_ARRAY;
        } elseif ($typeNull) {
            $compatibleType = static::T_NULL;
        } elseif ($typeSpecial) {
            // TODO: change after test
            $compatibleType = static::T_UNDEFINED;
        } elseif ($typeVoid) {
            $compatibleType = static::T_VOID;
        } elseif ($typeObject) {
            // TODO: change after test
            $compatibleType = static::T_UNDEFINED;
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

    private static function isTypeBoolCompatible(string $type): bool
    {
        return $type === static::T_BOOL;
    }

    private static function isTypeArrayCompatible(string $type): bool
    {
        return $type === static::T_ARRAY;
    }

    private static function isTypeNullCompatible(string $type): bool
    {
        return $type === static::T_NULL;
    }

    private static function isTypeSpecialCompatible(string $type): bool
    {
        switch ($type) {
            case static::T_NUMBER:
            case static::T_RESOURCE:
            case static::T_VARIABLE:
            case static::T_CALLABLE:
            case static::T_ITERABLE:
                return true;
            default:
                return false;
        }
    }

    private static function isTypeVoidCompatible(string $type): bool
    {
        return $type === static::T_VOID;
    }

    private static function isTypeObjectCompatible(string $type): bool
    {
        return $type === static::T_OBJECT;
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
