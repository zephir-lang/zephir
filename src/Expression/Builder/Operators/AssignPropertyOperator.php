<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder\Operators;

use Zephir\Expression\Builder\AbstractBuilder;

class AssignPropertyOperator extends AssignVariableOperator
{
    // a -> property = expr
    public const TYPE_ASSIGN_OBJECT_PROPERTY = 'object-property';

    // a :: property = expr
    public const TYPE_ASSIGN_STATIC_PROPERTY = 'static-property';
    private string $type = self::TYPE_ASSIGN_OBJECT_PROPERTY;

    public function __construct($variable = null, private $property = null, AbstractBuilder $expression = null)
    {
        parent::__construct($variable, $expression);

        if (null !== $property) {
            $this->setProperty($property);
        }
    }

    public function getProperty(): string
    {
        return $this->property;
    }

    public function setProperty(string $property): static
    {
        $this->property = $property;

        return $this;
    }

    public function getType(): string
    {
        return $this->type;
    }

    public function setType(string $type): static
    {
        $this->type = $type;

        return $this;
    }

    protected function preBuild(): array
    {
        $expression = parent::preBuild();
        $expression['assign-type'] = $this->getType();
        $expression['property'] = $this->getProperty();

        return $expression;
    }
}
