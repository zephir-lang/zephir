<?php

/**
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

use Test\TypeHinting\TestAbstract;

class TestConcreteClass extends TestAbstract
{
    public function testFunc(array $text = [], string $text2 = '', bool $flag = true, int $optional = null)
    {
        return func_get_args();
    }
}
