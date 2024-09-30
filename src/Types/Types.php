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

use Zephir\Class\Method\Method;

use function array_key_exists;
use function count;
use function in_array;

final class Types
{
    public const T_ARRAY    = 'array';
    public const T_BOOL     = 'bool';
    public const T_BOOLEAN  = 'boolean';
    public const T_CALLABLE = 'callable';
    public const T_CHAR     = 'char';
    public const T_DOUBLE   = 'double';
    public const T_FALSE    = 'false';
    public const T_FLOAT    = 'float';
    public const T_HASH     = 'hash';
    public const T_INT      = 'int';
    public const T_INTEGER  = 'integer';
    public const T_ISTRING  = 'istring';
    public const T_ITERABLE = 'iterable';
    public const T_LONG     = 'long';
    public const T_MIXED    = 'mixed';
    public const T_NULL     = 'null';
    public const T_NUMBER   = 'number';
    public const T_OBJECT   = 'object';
    public const T_RESOURCE = 'resource';
    public const T_STRING   = 'string';
    public const T_UCHAR    = 'uchar';
    public const T_UINT     = 'uint';
    public const T_ULONG    = 'ulong';
    public const T_VARIABLE = 'variable';
    public const T_VOID     = 'void';

    /**
     * Gets PHP compatible return type from class method.
     */
    public function getReturnTypeAnnotation(Method $method, array $returnTypes = null): string
    {
        if (!$method->hasReturnTypes() && !$method->isVoid()) {
            return '';
        }

        $isProcessedReturnType = null !== $returnTypes;
        $returnTypes           ??= $method->getReturnTypes();
        $typesCount            = count($returnTypes);

        $isDynamic  = in_array('var', array_keys($returnTypes));
        $isNullable = $this->isNullable($returnTypes);

        $isBool       = $this->areReturnTypesBoolCompatible($returnTypes);
        $isNull       = $this->areReturnTypesNullCompatible($returnTypes);
        $isVoid       = $this->areReturnTypesVoidCompatible($returnTypes);
        $isArray      = $this->areReturnTypesArrayCompatible($returnTypes);
        $isDouble     = $this->areReturnTypesFloatCompatible($returnTypes);
        $isString     = $this->areReturnTypesStringCompatible($returnTypes);
        $isObject     = $this->areReturnTypesObjectCompatible($returnTypes);
        $isInteger    = $this->areReturnTypesIntegerCompatible($returnTypes);
        $isNumeric    = $this->isNumeric($returnTypes);
        $isIterable   = $this->areReturnTypesIterableCompatible($returnTypes);
        $isResource   = $this->areReturnTypesResourceCompatible($returnTypes);
        $isCollection = $this->areReturnTypesCollectionCompatible($returnTypes);

        $isTypeHinted = $method->isReturnTypesHintDetermined();
        $isBasicTypes = $isArray || $isBool || $isDouble || $isInteger || $isResource || $isString || $isVoid || $isNumeric;

        $nullableType = $isNullable ? '|null' : '';

        if ($method->isVoid() || $isVoid) {
            return self::T_VOID;
        }

        if ($isInteger) {
            return self::T_INT . $nullableType;
        }

        if ($isDouble) {
            return self::T_FLOAT . $nullableType;
        }

        if ($isBool) {
            return self::T_BOOL . $nullableType;
        }

        if ($isString) {
            return self::T_STRING . $nullableType;
        }

        if ($isNull && 1 === $typesCount) {
            return self::T_NULL;
        }

        if ($isArray) {
            return self::T_ARRAY;
        }

        if ($isObject) {
            return self::T_OBJECT;
        }

        if ($isIterable) {
            return self::T_ITERABLE;
        }

        if ($isResource) {
            return self::T_RESOURCE;
        }

        if ($method->areReturnTypesCompatible() && !$isTypeHinted) {
            return self::T_MIXED . $nullableType;
        }

        if ($isTypeHinted && !$isBasicTypes && !$isDynamic && !$isNullable) {
            return implode('|', array_keys($returnTypes));
        }

        if ($isTypeHinted && $isProcessedReturnType) {
            $withoutNullable = array_filter(array_keys($returnTypes), static function ($ret) {
                if ($ret !== self::T_NULL) {
                    return $ret;
                }
            });

            return implode('|', array_values($withoutNullable)) . $nullableType;
        }

        if ($isCollection) {
            return implode('|', array_values($returnTypes));
        }

        return self::T_MIXED . $nullableType;
    }

    /**
     * Match Zephir types with Array type.
     */
    private function areReturnTypesArrayCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [self::T_ARRAY]);
    }

    /**
     * Match Zephir types with Boolean type.
     */
    private function areReturnTypesBoolCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [self::T_BOOL, self::T_BOOLEAN]);
    }

    /**
     * Match Zephir types with Collections.
     */
    private function areReturnTypesCollectionCompatible(array $types): bool
    {
        $result = false;

        foreach ($types as $type => $data) {
            if (str_contains($type, '[]')) {
                $result = true;
            }
        }

        return $result;
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
     */
    private function areReturnTypesCompatible(array $types, array $allowedTypes, bool $isNullable = false): bool
    {
        $result = null;

        if ($isNullable) {
            $allowedTypes[] = self::T_NULL;
        }

        foreach ($types as $type => $data) {
            $areEquals = in_array($type, $allowedTypes);

            $result = isset($result)
                ? ($areEquals && $result)
                : $areEquals;
        }

        return $result ?? false;
    }

    /**
     * Match Zephir types with Float type.
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
     * Match Zephir types with Integer type.
     */
    private function areReturnTypesIntegerCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible(
            $types,
            [
                self::T_INT,
                self::T_INTEGER,
                self::T_UINT,
                self::T_CHAR,
                self::T_UCHAR,
                self::T_LONG,
                self::T_ULONG,
            ]
        );
    }

    /**
     * Match Zephir types with Iterable type.
     */
    private function areReturnTypesIterableCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [self::T_ITERABLE]);
    }

    /**
     * Match Zephir types with Null type.
     */
    private function areReturnTypesNullCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [self::T_NULL]);
    }

    /**
     * Match Zephir types with Object type.
     */
    private function areReturnTypesObjectCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [self::T_OBJECT]);
    }

    /**
     * Match Zephir types with Resource type.
     */
    private function areReturnTypesResourceCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [self::T_RESOURCE]);
    }

    /**
     * Match Zephir types with String type.
     */
    private function areReturnTypesStringCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible(
            $types,
            [
                self::T_STRING,
                self::T_ISTRING,
            ],
            $this->isNullable($types)
        );
    }

    /**
     * Match Zephir types with Void type.
     */
    private function areReturnTypesVoidCompatible(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [self::T_VOID]);
    }

    /**
     * Check if Zephir types can be Nullable.
     */
    private function isNullable(array $types): bool
    {
        return (array_key_exists(self::T_NULL, $types)
                || in_array(self::T_NULL, $types))
            && 1 !== count($types);
    }

    /**
     * Check if Zephir types is a Numeric type compatible.
     */
    private function isNumeric(array $types): bool
    {
        return $this->areReturnTypesCompatible($types, [self::T_NUMBER]);
    }
}
