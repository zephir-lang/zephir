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

namespace Zephir\Expression\Builder\Operators;

use Zephir\Expression\Builder\AbstractBuilder;

/**
 * Allows to manually build a binary operator AST node
 */
class BinaryOperator extends AbstractOperator
{
    // x + y
    public const OPERATOR_ACCESS_ARRAY = 'array-access';

    // x - y
    public const OPERATOR_ACCESS_PROPERTY = 'property-access';

    // x * y
    public const OPERATOR_ACCESS_PROPERTY_DYNAMIC = 'property-dynamic-access';

    // x / y
    public const OPERATOR_ACCESS_PROPERTY_STRING = 'property-string-access';

    // x % y
    public const OPERATOR_ACCESS_STATIC_CONSTANT = 'static-constant-access';

    // x . y
    public const OPERATOR_ACCESS_STATIC_PROPERTY = 'static-property-access';

    // x && y
    public const OPERATOR_ADD = 'add';

    // x || y
    public const OPERATOR_AND = 'and';

    // x | y
    public const OPERATOR_BITWISE_AND = 'bitwise_and';

    // x & y
    public const OPERATOR_BITWISE_OR = 'bitwise_or';

    // x ^ y
    public const OPERATOR_BITWISE_SHIFT_LEFT = 'bitwise_shiftleft';

    // x << y
    public const OPERATOR_BITWISE_SHIFT_RIGHT = 'bitwise_shiftright';

    // x >> y
    public const OPERATOR_BITWISE_XOR = 'bitwise_xor';

    // x instanceof y
    public const OPERATOR_CAST = 'cast';

    // x .. y
    public const OPERATOR_CONCAT = 'concat';

    // x ... y
    public const OPERATOR_DIV = 'div';

    // x == y
    public const OPERATOR_EQUALS = 'equals';

    // x != y
    public const OPERATOR_ERANGE = 'erange';

    // x ==- y
    public const OPERATOR_GREATER = 'greater';

    // x !== y
    public const OPERATOR_GREATER_EQUAL = 'greater-equal';

    // x < y
    public const OPERATOR_IDENTICAL = 'identical';

    // x > y
    public const OPERATOR_INSTANCEOF = 'instanceof';

    // x <= y
    public const OPERATOR_IRANGE = 'irange';

    // x >= y
    public const OPERATOR_LESS = 'less';

    // (type) a
    public const OPERATOR_LESS_EQUAL = 'less-equal';

    // <type> a
    public const OPERATOR_MOD = 'mod';

    // x -> y
    public const OPERATOR_MUL = 'mul';

    // x -> {y}
    public const OPERATOR_NOT_EQUALS = 'not-equals';

    // x -> {"string"}
    public const OPERATOR_NOT_IDENTICAL = 'not-identical';

    // x :: y
    public const OPERATOR_OR = 'or';

    // x :: CONSTANT
    public const OPERATOR_SUB = 'sub';

    // x [{expr}]
    public const OPERATOR_TYPE_HINT = 'type-hint';

    public function __construct(
        private $operator = null,
        private ?AbstractBuilder $leftExpression = null,
        private ?AbstractBuilder $rightExpression = null
    ) {
        if (null !== $operator) {
            $this->setOperator($operator);
        }

        if (null !== $leftExpression) {
            $this->setLeftExpression($leftExpression);
        }

        if (null !== $rightExpression) {
            $this->setRightExpression($rightExpression);
        }
    }

    public function getLeftExpression(): AbstractBuilder
    {
        return $this->leftExpression;
    }

    public function getOperator(): string
    {
        return $this->operator;
    }

    public function getRightExpression(): AbstractBuilder
    {
        return $this->rightExpression;
    }

    public function setLeftExpression(AbstractBuilder $leftExpression): BinaryOperator
    {
        $this->leftExpression = $leftExpression;

        return $this;
    }

    public function setOperator(string $operator): BinaryOperator
    {
        $this->operator = $operator;

        return $this;
    }

    public function setRightExpression(AbstractBuilder $rightExpression): BinaryOperator
    {
        $this->rightExpression = $rightExpression;

        return $this;
    }

    protected function preBuild(): array
    {
        return [
            'type'  => $this->getOperator(),
            'left'  => $this->getLeftExpression(),
            'right' => $this->getRightExpression(),
        ];
    }
}
