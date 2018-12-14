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
use const Zephir\T_ISTRING;
use const Zephir\T_STRING;

final class StringCompatible extends ReturnType\CompositeSpecification
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
            case T_STRING:
            case T_ISTRING:
                return true;
            default:
                return false;
        }
    }
}
