<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder\Factory;

use Zephir\Expression\Builder\AbstractBuilder;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Expression\Builder\Operators\AssignPropertyOperator;
use Zephir\Expression\Builder\Operators\AssignVariableOperator;
use Zephir\Expression\Builder\Operators\BinaryOperator;
use Zephir\Expression\Builder\Operators\RawOperator;
use Zephir\Expression\Builder\Operators\UnaryOperator;

/**
 * Class OperatorsFactory.
 */
class OperatorsFactory
{
    protected $factory;
    protected $assignFactory;

    /**
     * @param BuilderFactory $factory
     */
    public function __construct(BuilderFactory $factory)
    {
        $this->factory = $factory;
    }

    /**
     * @param null                 $variable
     * @param AbstractBuilder|null $expression
     *
     * @return AssignVariableOperator
     */
    public function assignVariable($variable = null, AbstractBuilder $expression = null)
    {
        return new AssignVariableOperator($variable, $expression);
    }

    /**
     * @param null                 $variable
     * @param null                 $property
     * @param AbstractBuilder|null $expression
     *
     * @return AssignPropertyOperator
     */
    public function assignProperty($variable = null, $property = null, AbstractBuilder $expression = null)
    {
        $assignProperty = new AssignPropertyOperator($variable, $property, $expression);
        $assignProperty->setType($assignProperty::TYPE_ASSIGN_OBJECT_PROPERTY);

        return $assignProperty;
    }

    /**
     * @param null                 $variable
     * @param null                 $property
     * @param AbstractBuilder|null $expression
     *
     * @return AssignPropertyOperator
     */
    public function assignStaticProperty($variable = null, $property = null, AbstractBuilder $expression = null)
    {
        $assignProperty = $this->assignProperty($variable, $property, $expression);
        $assignProperty->setType($assignProperty::TYPE_ASSIGN_STATIC_PROPERTY);

        return $assignProperty;
    }

    /**
     * @param null                 $operator
     * @param AbstractBuilder|null $leftExpression
     * @param AbstractBuilder|null $rightExpression
     *
     * @return BinaryOperator
     */
    public function binary($operator = null, AbstractBuilder $leftExpression = null, AbstractBuilder $rightExpression = null)
    {
        return new BinaryOperator($operator, $leftExpression, $rightExpression);
    }

    /**
     * @param string|null          $operator
     * @param AbstractBuilder|null $expression
     *
     * @return UnaryOperator
     */
    public function unary($operator = null, AbstractBuilder $expression = null)
    {
        return new UnaryOperator($operator, $expression);
    }

    /**
     * @param $type
     * @param AbstractBuilder $expression
     *
     * @return RawOperator
     */
    public function cast($type, AbstractBuilder $expression)
    {
        return $this->raw([
            'type' => BinaryOperator::OPERATOR_CAST,
            'left' => $type,
            'right' => $expression,
        ]);
    }

    /**
     * @param $type
     * @param AbstractBuilder $expression
     *
     * @return RawOperator
     */
    public function hint($type, AbstractBuilder $expression)
    {
        return $this->raw([
            'type' => BinaryOperator::OPERATOR_TYPE_HINT,
            'left' => $type,
            'right' => $expression,
        ]);
    }

    /**
     * @param array $expression
     *
     * @return RawOperator
     */
    public function raw(array $expression)
    {
        return new RawOperator($expression);
    }
}
