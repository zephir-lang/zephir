<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Test\ArrayObject;

class ArrayObjectTest extends TestCase
{
    public function testSetGet()
    {
        $t = new ArrayObject();
        $this->assertInstanceOf('\ArrayObject', $t);

        $t->test_1 = 1;
        $this->assertSame(1, $t->test_1);

        $t['test_2'] = 1;
        $this->assertSame(1, $t['test_2']);
    }
}
