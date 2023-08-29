<?php

/**
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
    public function isSatisfiedBy(ReturnType\TypeInterface $type): bool
    {
        if (!$type->isRealType()) {
            return false;
        }

        return match ($type->getDataType()) {
            Types::T_INT, Types::T_UINT, Types::T_CHAR, Types::T_UCHAR, Types::T_LONG, Types::T_ULONG => true,
            default => false,
        };
    }
}
