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

    const COMPATIBLETYPES = [
        'int' => [
            self::T_INT,
            self::T_UINT,
            self::T_CHAR,
            self::T_UCHAR,
            self::T_LONG,
            self::T_ULONG,
        ],
        'float' => [
            self::T_FLOAT,
            self::T_DOUBLE,
        ],
        'bool' => [
            self::T_BOOL,
        ],
        'string' => [
            self::T_STRING,
            self::T_ISTRING,
        ],
        'null' => [
            self::T_NULL,
        ],
        'array' => [
            self::T_ARRAY,
        ],
        'object' => [
            self::T_OBJECT,
        ],
        'iterable' => [
            self::T_ITERABLE,
        ],
        'resource' => [
            self::T_RESOURCE,
        ],
        'void' => [
            self::T_VOID,
        ],
    ];

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

        $isNullable = $method->areReturnTypesNullCompatible()
            && 1 !== $typesCount;

        $isInteger = $this->areReturnTypesCompatible($returnTypes, static::COMPATIBLETYPES['int']);
        $isDouble = $this->areReturnTypesCompatible($returnTypes, static::COMPATIBLETYPES['float']);
        $isBool = $this->areReturnTypesCompatible($returnTypes, static::COMPATIBLETYPES['bool']);
        $isString = $this->areReturnTypesCompatible($returnTypes, static::COMPATIBLETYPES['string'], $isNullable);
        $isNull = $this->areReturnTypesCompatible($returnTypes, static::COMPATIBLETYPES['null']);
        $isArray = $this->areReturnTypesCompatible($returnTypes, static::COMPATIBLETYPES['array']);
        $isObject = $this->areReturnTypesCompatible($returnTypes, static::COMPATIBLETYPES['object']);
        $isIterable = $this->areReturnTypesCompatible($returnTypes, static::COMPATIBLETYPES['iterable']);
        $isResource = $this->areReturnTypesCompatible($returnTypes, static::COMPATIBLETYPES['resource']);
        $isVoid = $this->areReturnTypesCompatible($returnTypes, static::COMPATIBLETYPES['void']);
        $isNumeric = $this->areReturnTypesCompatible($returnTypes, [static::T_NUMBER]);
        $isDynamic = \in_array('var', array_keys($returnTypes));

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

        return static::T_MIXED.$nullableType;
    }

    /**
     * Match if return types from Zephir are compatible
     * with allowed return types from PHP.
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
