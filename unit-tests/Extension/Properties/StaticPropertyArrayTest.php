<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Properties;

use Test\Properties\StaticPropertyArray;
use Zephir\Support\TestCase;

class StaticPropertyArrayTest extends TestCase
{
    public function testAssertations()
    {
        $this->assertSame([], StaticPropertyArray::$someEmptyArray);
        $this->assertSame([1, 2, 3, 4], StaticPropertyArray::$someArray);
    }
}
