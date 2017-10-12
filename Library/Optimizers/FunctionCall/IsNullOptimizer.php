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

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Optimizers\IsTypeOptimizerAbstract;

/**
 * IsNullOptimizer
 *
 * Optimizes calls to 'is_null' using internal function
 */
class IsNullOptimizer extends IsTypeOptimizerAbstract
{
    protected function getType()
    {
        return 'IS_NULL';
    }
}
