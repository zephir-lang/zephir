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
use const Zephir\T_CHAR;
use const Zephir\T_INT;
use const Zephir\T_LONG;
use const Zephir\T_UCHAR;
use const Zephir\T_UINT;
use const Zephir\T_ULONG;

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
        if (false == $type->isReal()) {
            return false;
        }

        switch ($type->getDataType()) {
            case T_INT:
            case T_UINT:
            case T_CHAR:
            case T_UCHAR:
            case T_LONG:
            case T_ULONG:
                return true;
            default:
                return false;
        }
    }
}
