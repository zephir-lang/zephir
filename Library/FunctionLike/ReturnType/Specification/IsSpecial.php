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

/**
 * Special types specification.
 *
 * Used only for type hinting.
 */
final class IsSpecial extends ReturnType\CompositeSpecification
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
        switch ($type->getDataType()) {
            case Types::T_NUMBER:
            case Types::T_RESOURCE:
            case Types::T_VARIABLE:
            case Types::T_CALLABLE:
            case Types::T_ITERABLE:
                return true;
            default:
                return false;
        }
    }
}
