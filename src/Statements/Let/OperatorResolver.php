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

namespace Zephir\Statements\Let;

use Zephir\CompiledExpression;
use Zephir\Exception\IllegalOperationException;
use Zephir\Variable\Variable;

/**
 * Resolves assignment operators to their string representations.
 * Consolidates repeated operator matching logic.
 */
final class OperatorResolver
{
    /**
     * Basic arithmetic operators supported for numeric assignments
     */
    public const NUMERIC_OPERATORS = [
        'assign' => ' = ',
        'add-assign' => ' += ',
        'sub-assign' => ' -= ',
        'mul-assign' => ' *= ',
        'div-assign' => ' /= ',
        'mod-assign' => ' %= ',
    ];

    /**
     * Limited operators for char types
     */
    public const CHAR_OPERATORS = [
        'assign' => ' = ',
        'add-assign' => ' += ',
        'sub-assign' => ' -= ',
        'mul-assign' => ' *= ',
    ];

    /**
     * Limited operators for bool types
     */
    public const BOOL_OPERATORS = [
        'assign' => ' = ',
        'add-assign' => ' += ',
        'sub-assign' => ' -= ',
    ];

    /**
     * Resolve numeric operator
     */
    public static function resolveNumeric(string $operator, array $statement, CompiledExpression $resolvedExpr): string
    {
        if (!isset(self::NUMERIC_OPERATORS[$operator])) {
            throw new IllegalOperationException($statement, $resolvedExpr);
        }

        return self::NUMERIC_OPERATORS[$operator];
    }

    /**
     * Resolve char operator
     */
    public static function resolveChar(string $operator, array $statement, CompiledExpression $resolvedExpr): string
    {
        if (!isset(self::CHAR_OPERATORS[$operator])) {
            throw new IllegalOperationException($statement, $resolvedExpr);
        }

        return self::CHAR_OPERATORS[$operator];
    }

    /**
     * Resolve bool operator
     *
     * @throws IllegalOperationException
     */
    public static function resolveBool(string $operator, array $statement, CompiledExpression $resolvedExpr): string
    {
        if (!isset(self::BOOL_OPERATORS[$operator])) {
            throw new IllegalOperationException($statement, $resolvedExpr);
        }

        return self::BOOL_OPERATORS[$operator];
    }

    /**
     * Resolve variable operator (for variables)
     *
     * @throws IllegalOperationException
     */
    public static function resolveVariable(string $operator, array $statement, Variable $itemVariable): string
    {
        if (!isset(self::NUMERIC_OPERATORS[$operator])) {
            throw new IllegalOperationException($statement, $itemVariable);
        }

        return self::NUMERIC_OPERATORS[$operator];
    }

    /**
     * Check if operator is assignment only
     */
    public static function isSimpleAssignment(string $operator): bool
    {
        return $operator === 'assign';
    }
}
