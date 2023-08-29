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

final class ArrayCompatible extends ReturnType\CompositeSpecification
{
    public function isSatisfiedBy(ReturnType\TypeInterface $type): bool
    {
        if ($type->isRealType()) {
            return Types::T_ARRAY === $type->getDataType();
        }

        return $type->isCollection();
    }
}
