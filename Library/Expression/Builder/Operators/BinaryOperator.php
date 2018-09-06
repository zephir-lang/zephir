<?php
/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/
namespace Zephir\Expression\Builder\Operators;

use Zephir\Expression\Builder\AbstractBuilder;

/**
 * BinaryOperator
 *
 * Allows to manually build a binary operator AST node
 */
class BinaryOperator extends AbstractOperator
{
    // x + y
    const OPERATOR_ADD = 'add';

    // x - y
    const OPERATOR_SUB = 'sub';

    // x * y
    const OPERATOR_MUL = 'mul';

    // x / y
    const OPERATOR_DIV = 'div';

    // x % y
    const OPERATOR_MOD = 'mod';

    // x . y
    const OPERATOR_CONCAT = 'concat';

    // x && y
    const OPERATOR_AND = 'and';

    // x || y
    const OPERATOR_OR = 'or';

    // x | y
    const OPERATOR_BITWISE_OR = 'bitwise_or';

    // x & y
    const OPERATOR_BITWISE_AND = 'bitwise_and';

    // x ^ y
    const OPERATOR_BITWISE_XOR = 'bitwise_xor';

    // x << y
    const OPERATOR_BITWISE_SHIFT_LEFT = 'bitwise_shiftleft';

    // x >> y
    const OPERATOR_BITWISE_SHIFT_RIGHT = 'bitwise_shiftright';

    // x instanceof y
    const OPERATOR_INSTANCEOF = 'instanceof';

    // x .. y
    const OPERATOR_IRANGE = 'irange';

    // x ... y
    const OPERATOR_ERANGE = 'erange';

    // x == y
    const OPERATOR_EQUALS = 'equals';

    // x != y
    const OPERATOR_NOT_EQUALS = 'not-equals';

    // x ==- y
    const OPERATOR_IDENTICAL = 'identical';

    // x !== y
    const OPERATOR_NOT_IDENTICAL = 'not-identical';

    // x < y
    const OPERATOR_LESS = 'less';

    // x > y
    const OPERATOR_GREATER = 'greater';

    // x <= y
    const OPERATOR_LESS_EQUAL = 'less-equal';

    // x >= y
    const OPERATOR_GREATER_EQUAL = 'greater-equal';

    // (type) a
    const OPERATOR_CAST = 'cast';

    // <type> a
    const OPERATOR_TYPE_HINT = 'type-hint';

    // x -> y
    const OPERATOR_ACCESS_PROPERTY = 'property-access';

    // x -> {y}
    const OPERATOR_ACCESS_PROPERTY_DYNAMIC = 'property-dynamic-access';

    // x -> {"string"}
    const OPERATOR_ACCESS_PROPERTY_STRING = 'property-string-access';

    // x :: y
    const OPERATOR_ACCESS_STATIC_PROPERTY = 'static-property-access';

    // x :: CONSTANT
    const OPERATOR_ACCESS_STATIC_CONSTANT = 'static-constant-access';

    // x [{expr}]
    const OPERATOR_ACCESS_ARRAY = 'array-access';


    private $operator;
    private $leftExpression;
    private $rightExpression;

    /**
     * @param null $operator
     * @param AbstractBuilder|null $leftExpression
     * @param AbstractBuilder|null $rightExpression
     */
    public function __construct($operator = null, AbstractBuilder $leftExpression = null, AbstractBuilder $rightExpression = null)
    {
        if ($operator !== null) {
            $this->setOperator($operator);
        }

        if ($leftExpression !== null) {
            $this->setLeftExpression($leftExpression);
        }

        if ($rightExpression !== null) {
            $this->setRightExpression($rightExpression);
        }
    }

    /**
     * @return string
     */
    public function getOperator()
    {
        return $this->operator;
    }

    /**
     * @param string $operator
     * @return BinaryOperator
     */
    public function setOperator($operator)
    {
        $this->operator = $operator;
        return $this;
    }

    /**
     * @return AbstractBuilder
     */
    public function getLeftExpression()
    {
        return $this->leftExpression;
    }

    /**
     * @param AbstractBuilder $leftExpression
     * @return BinaryOperator
     */
    public function setLeftExpression(AbstractBuilder $leftExpression)
    {
        $this->leftExpression = $leftExpression;
        return $this;
    }

    /**
     * @return AbstractBuilder
     */
    public function getRightExpression()
    {
        return $this->rightExpression;
    }

    /**
     * @param AbstractBuilder $rightExpression
     * @return BinaryOperator
     */
    public function setRightExpression(AbstractBuilder $rightExpression)
    {
        $this->rightExpression = $rightExpression;
        return $this;
    }

    /**
     * {@inheritdoc}
     */
    protected function preBuild()
    {
        return array(
            'type'  => $this->getOperator(),
            'left'  => $this->getLeftExpression(),
            'right' => $this->getRightExpression()
        );
    }
}
