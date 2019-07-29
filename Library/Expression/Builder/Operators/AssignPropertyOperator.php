<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder\Operators;

use Zephir\Expression\Builder\AbstractBuilder;

/**
 * Class AssignPropertyOperator.
 */
class AssignPropertyOperator extends AssignVariableOperator
{
    // a -> property = expr
    const TYPE_ASSIGN_OBJECT_PROPERTY = 'object-property';

    // a :: property = expr
    const TYPE_ASSIGN_STATIC_PROPERTY = 'static-property';

    private $property;
    private $type = self::TYPE_ASSIGN_OBJECT_PROPERTY;

    /**
     * @param null                 $variable
     * @param null                 $property
     * @param AbstractBuilder|null $expression
     */
    public function __construct($variable = null, $property = null, AbstractBuilder $expression = null)
    {
        parent::__construct($variable, $expression);

        if (null !== $property) {
            $this->setProperty($property);
        }
    }

    /**
     * @return string
     */
    public function getProperty()
    {
        return $this->property;
    }

    /**
     * @param string $property
     *
     * @return $this
     */
    public function setProperty($property)
    {
        $this->property = $property;

        return $this;
    }

    /**
     * @return string
     */
    public function getType()
    {
        return $this->type;
    }

    /**
     * @param string $type
     *
     * @return AssignPropertyOperator
     */
    public function setType($type)
    {
        $this->type = $type;

        return $this;
    }

    /**
     * {@inheritdoc}
     */
    protected function preBuild()
    {
        $expression = parent::preBuild();
        $expression['assign-type'] = $this->getType();
        $expression['property'] = $this->getProperty();

        return $expression;
    }
}
