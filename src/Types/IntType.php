<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Types;

/**
 * Defines methods of the built-in int type
 */
class IntType extends AbstractType
{
    use NumericTypeTrait;

    public array $methodMap = [];

    public function __construct()
    {
        $this->methodMap = $this->getNumericMethodMap();
    }

    public function getTypeName(): string
    {
        return Types::T_INT;
    }
}
