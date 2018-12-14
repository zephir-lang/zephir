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
use const Zephir\T_CALLABLE;
use const Zephir\T_NULL;
use const Zephir\T_OBJECT;
use const Zephir\T_RESOURCE;
use const Zephir\T_VARIABLE;

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
            case T_OBJECT:
            case T_RESOURCE:
            case T_NULL:
            case T_VARIABLE:
            case T_CALLABLE:
                return true;
            default:
                return false;
        }
    }
}
