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

final class Not extends CompositeSpecification
{
    /**
     * @var SpecificationInterface
     */
    protected $spec;

    /**
     * Creates a new specification wrapping another.
     *
     * @param SpecificationInterface $spec
     */
    public function __construct(SpecificationInterface $spec)
    {
        $this->spec = $spec;
    }

    /**
     * Returns the negated result of the wrapped specification.
     *
     * @param ReturnType\TypeInterface $type
     *
     * @return bool
     */
    public function isSatisfiedBy(ReturnType\TypeInterface $type)
    {
        return false == $this->spec->isSatisfiedBy($type);
    }
}
