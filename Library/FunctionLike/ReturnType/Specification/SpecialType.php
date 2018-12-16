<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\FunctionLike\ReturnType\Specification;

use Zephir\FunctionLike\ReturnType;
use Zephir\Types;

final class SpecialType extends ReturnType\CompositeSpecification
{
    /**
     * {@inheritdoc}
     *
     * @param ReturnType\TypeInterface $type
     *
     * @return bool
     */
    public function isSatisfiedBy(ReturnType\TypeInterface $type)
    {
        if (false == $type->isReal()) {
            return false;
        }

        switch ($type->getDataType()) {
            case Types::T_OBJECT:
            case Types::T_RESOURCE:
            case Types::T_NULL:
            case Types::T_VARIABLE:
            case Types::T_CALLABLE:
                return true;
            default:
                return false;
        }
    }
}
