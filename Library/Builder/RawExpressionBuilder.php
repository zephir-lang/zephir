<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Builder;

/**
 * RawExpressionBuilder
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
