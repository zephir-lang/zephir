<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\FunctionLike\ReturnType\Specification;

use Zephir\FunctionLike\ReturnType;
use Zephir\Types;

final class IntCompatible extends ReturnType\CompositeSpecification
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
        if (false == $type->isRealType()) {
            return false;
        }

        switch ($type->getDataType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_CHAR:
            case Types::T_UCHAR:
            case Types::T_LONG:
            case Types::T_ULONG:
                return true;
            default:
                return false;
        }
    }
}
