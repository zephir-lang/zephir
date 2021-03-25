<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\FunctionLike\ReturnType;

use Zephir\FunctionLike\ReturnType;

abstract class CompositeSpecification implements SpecificationInterface
{
    /**
     * Creates a new logical OR composite specification.
     *
     * @param SpecificationInterface $spec
     *
     * @return Either
     */
    public function either(SpecificationInterface $spec)
    {
        return new Either($this, $spec);
    }

    /**
     * Creates a new logical AND specification.
     *
     * @param SpecificationInterface $spec
     *
     * @return Plus
     */
    public function plus(SpecificationInterface $spec)
    {
        return new Plus($this, $spec);
    }

    /**
     * Creates a new logical NOT specification.
     *
     * @return Not
     */
    public function not()
    {
        return new Not($this);
    }

    /**
     * {@inheritdoc}
     *
     * @param ReturnType\TypeInterface $type
     *
     * @return bool
     */
    abstract public function isSatisfiedBy(ReturnType\TypeInterface $type);
}
