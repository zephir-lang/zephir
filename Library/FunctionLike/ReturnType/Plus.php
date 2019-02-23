<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\FunctionLike\ReturnType;

use Zephir\FunctionLike\ReturnType;

final class Plus extends CompositeSpecification
{
    /**
     * Left Specification.
     *
     * @var SpecificationInterface
     */
    protected $left;

    /**
     * Right Specification.
     *
     * @var SpecificationInterface
     */
    protected $right;

    /**
     * A composite wrapper of two specifications.
     *
     * @param SpecificationInterface $left
     * @param SpecificationInterface $right
     */
    public function __construct(SpecificationInterface $left, SpecificationInterface $right)
    {
        $this->left = $left;
        $this->right = $right;
    }

    /**
     * Checks if the composite AND of specifications passes.
     *
     * @param ReturnType\TypeInterface $type
     *
     * @return bool
     */
    public function isSatisfiedBy(ReturnType\TypeInterface $type)
    {
        return $this->left->isSatisfiedBy($type) && $this->right->isSatisfiedBy($type);
    }
}
