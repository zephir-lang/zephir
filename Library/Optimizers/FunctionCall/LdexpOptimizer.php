<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Expression;
use Zephir\Optimizers\MathOptimizer;

/**
 * LdexpOptimizer
 *
 * Optimizes calls to 'ldexp' using internal function
 *
 * @link http://www.codecogs.com/library/computing/c/math.h/ldexp.php
 * @link https://github.com/php/php-src/pull/1489
 */
class LdexpOptimizer extends MathOptimizer
{
    /**
     * {@inheritdoc}
     */
    public function getParameters()
    {
        return array(
            array(
                'name' => 'value',
                'type' => 'mixed'
            ),
            array(
                'name' => 'exp',
                'type' => 'integer'
            )
        );
    }

    public function getFunctionName()
    {
        return 'ldexp';
    }
}
