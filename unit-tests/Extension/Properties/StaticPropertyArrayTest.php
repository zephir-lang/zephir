<?php

/**
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
        $this->assertSame(array(), StaticPropertyArray::$someEmptyArray);
        $this->assertSame(array(1, 2, 3, 4), StaticPropertyArray::$someArray);
    }
}
