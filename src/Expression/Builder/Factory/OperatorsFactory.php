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

namespace Zephir\Expression\Builder\Factory;

use Zephir\Expression\Builder\AbstractBuilder;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Expression\Builder\Operators\AssignPropertyOperator;
use Zephir\Expression\Builder\Operators\AssignVariableOperator;
use Zephir\Expression\Builder\Operators\BinaryOperator;
use Zephir\Expression\Builder\Operators\RawOperator;
use Zephir\Expression\Builder\Operators\UnaryOperator;

class OperatorsFactory
{
    public function __construct(protected BuilderFactory $factory)
    {
    }

    public function assignProperty(
        $variable = null,
        $property = null,
        ?AbstractBuilder $expression = null
    ): AssignPropertyOperator {
        $assignProperty = new AssignPropertyOperator($variable, $property, $expression);
        $assignProperty->setType($assignProperty::TYPE_ASSIGN_OBJECT_PROPERTY);

        return $assignProperty;
    }

    public function assignStaticProperty(
        $variable = null,
        $property = null,
        ?AbstractBuilder $expression = null
    ): AssignPropertyOperator {
        $assignProperty = $this->assignProperty($variable, $property, $expression);
        $assignProperty->setType($assignProperty::TYPE_ASSIGN_STATIC_PROPERTY);

        return $assignProperty;
    }

    public function assignVariable(
        ?string $variable = null,
        ?AbstractBuilder $expression = null
    ): AssignVariableOperator {
        return new AssignVariableOperator($variable, $expression);
    }

    public function binary(
        $operator = null,
        ?AbstractBuilder $leftExpression = null,
        ?AbstractBuilder $rightExpression = null
    ): BinaryOperator {
        return new BinaryOperator($operator, $leftExpression, $rightExpression);
    }

    public function cast(string $type, AbstractBuilder $expression): RawOperator
    {
        return $this->raw([
            'type'  => BinaryOperator::OPERATOR_CAST,
            'left'  => $type,
            'right' => $expression,
        ]);
    }

    public function hint(string $type, AbstractBuilder $expression): RawOperator
    {
        return $this->raw([
            'type'  => BinaryOperator::OPERATOR_TYPE_HINT,
            'left'  => $type,
            'right' => $expression,
        ]);
    }

    public function raw(array $expression): RawOperator
    {
        return new RawOperator($expression);
    }

    public function unary(?string $operator = null, ?AbstractBuilder $expression = null): UnaryOperator
    {
        return new UnaryOperator($operator, $expression);
    }
}
