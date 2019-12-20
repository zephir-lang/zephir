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

    /**
     * Gets PHP compatible return type from class method.
     *
     * @param ClassMethod $method
     *
     * @return string
     */
    public function getReturnTypeAnnotation(ClassMethod $method, array $returnTypes = null): string
    {
        if (!$method->hasReturnTypes() && !$method->isVoid()) {
            return '';
        }

        $returnTypes = $returnTypes ?? $method->getReturnTypes();
        $typesCount = \count($returnTypes);

        $isDynamic = \in_array('var', array_keys($returnTypes));
        $isNullable = $this->isNullable($returnTypes);

        $isBool = $this->areReturnTypesBoolCompatible($returnTypes);
        $isNull = $this->areReturnTypesNullCompatible($returnTypes);
        $isVoid = $this->areReturnTypesVoidCompatible($returnTypes);
        $isArray = $this->areReturnTypesArrayCompatible($returnTypes);
        $isDouble = $this->areReturnTypesFloatCompatible($returnTypes);
        $isString = $this->areReturnTypesStringCompatible($returnTypes);
        $isObject = $this->areReturnTypesObjectCompatible($returnTypes);
        $isInteger = $this->areReturnTypesIntegerCompatible($returnTypes);
        $isNumeric = $this->isNumeric($returnTypes);
        $isIterable = $this->areReturnTypesIterableCompatible($returnTypes);
        $isResource = $this->areReturnTypesResourceCompatible($returnTypes);
        $isCollection = $this->areReturnTypesCollectionCompatible($returnTypes);

        $isTypeHinted = $method->isReturnTypesHintDetermined();
        $isBasicTypes = $isArray || $isBool || $isDouble || $isInteger || $isResource || $isString || $isVoid || $isNumeric;

        $nullableType = $isNullable ? '|null' : '';

        if ($method->isVoid() || $isVoid) {
            return static::T_VOID;
        }

        if ($isInteger) {
            return static::T_INT.$nullableType;
        }

        if ($isDouble) {
            return static::T_FLOAT.$nullableType;
        }

        if ($isBool) {
            return static::T_BOOL.$nullableType;
        }

        if ($isString) {
            return static::T_STRING.$nullableType;
        }

        if ($isNull && 1 === $typesCount) {
            return static::T_NULL;
        }

        if ($isArray) {
            return static::T_ARRAY;
        }

        if ($isObject) {
            return static::T_OBJECT;
        }

        if ($isIterable) {
            return static::T_ITERABLE;
        }

        if ($isResource) {
            return static::T_RESOURCE;
        }

        if ($method->areReturnTypesCompatible() && !$isTypeHinted) {
            return static::T_MIXED.$nullableType;
        }

        if ($isTypeHinted && !$isBasicTypes && !$isDynamic) {
            return implode('|', array_keys($returnTypes));
        }

        if ($isCollection) {
            return implode('|', array_values($returnTypes));
        }

        return static::T_MIXED.$nullableType;
    }

    /**
     * Match Zephir types with Integer type.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesIntegerCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible(
            $types,
            [
                self::T_INT,
                self::T_UINT,
                self::T_CHAR,
                self::T_UCHAR,
                self::T_LONG,
                self::T_ULONG,
            ]
        );
    }

    /**
     * Match Zephir types with Float type.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesFloatCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible(
            $types,
            [
                self::T_FLOAT,
                self::T_DOUBLE,
            ]
        );
    }

    /**
     * Match Zephir types with Boolean type.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesBoolCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [static::T_BOOL]);
    }

    /**
     * Match Zephir types with String type.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesStringCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible(
            $types,
            [
                static::T_STRING,
                static::T_ISTRING,
            ],
            $this->isNullable($types)
        );
    }

    /**
     * Match Zephir types with Null type.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesNullCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [static::T_NULL]);
    }

    /**
     * Match Zephir types with Array type.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesArrayCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [static::T_ARRAY]);
    }

    /**
     * Match Zephir types with Object type.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesObjectCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [static::T_OBJECT]);
    }

    /**
     * Match Zephir types with Iterable type.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesIterableCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [static::T_ITERABLE]);
    }

    /**
     * Match Zephir types with Resource type.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesResourceCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [static::T_RESOURCE]);
    }

    /**
     * Match Zephir types with Collections.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesCollectionCompatible(array $types): bool
    {
        $result = false;

        foreach ($types as $type => $data) {
            if (false !== strpos($type, '[]')) {
                $result = true;
            }
        }

        return $result;
    }

    /**
     * Match Zephir types with Void type.
     *
     * @param array $types
     *
     * @return bool
     */
    private function areReturnTypesVoidCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [static::T_VOID]);
    }

    /**
     * Check if Zephir types is a Numeric type compatible.
     *
     * @param array $types
     *
     * @return bool
     */
    private function isNumeric(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [static::T_NUMBER]);
    }

    /**
     * Check if Zephir types can be Nullable.
     *
     * @param array $types
     *
     * @return bool
     */
    private function isNullable(array $types): bool
    {
        return \array_key_exists(static::T_NULL, $types)
            && 1 !== \count($types);
    }

    /**
     * Match if return types from Zephir are compatible
     * with allowed return types from PHP.
     *
     * Examples:
     *  $types = [
     *      'variable' => [
     *          'type' => 'return-type-parameter',
     *          'data-type' => 'variable',
     *          'mandatory' => 0,
     *          'file' => '../path-to-file/stubs.zep',
     *          'line' => 21,
     *          'char' => 48,
     *       ]
     *  ]
     *
     * @param array $types        - Return types from parser
     * @param array $allowedTypes - Allowed return types
     *
     * @return bool
     */
    private function areReturnTypesCompatible(array $types, array $allowedTypes, bool $isNullable = false): bool
    {
        $result = null;
        $areEquals = false;

        if ($isNullable) {
            array_push($allowedTypes, static::T_NULL);
        }

        foreach ($types as $type => $data) {
            $areEquals = \in_array($type, $allowedTypes);

            $result = isset($result)
                ? ($areEquals && $result)
                : $areEquals;
        }

        return $result ?? false;
    }
}
