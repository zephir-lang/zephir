<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder;

/**
 * RawExpressionBuilder.
 *
 * Allows to use a raw expression in a builder
 */
class RawExpressionBuilder
{
    protected $expr;

    public function __construct($expr)
    {
        $this->expr = $expr;
    }

    public function get()
    {
        return $this->expr;
    }
}
