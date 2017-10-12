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

namespace Extension;

use Test\ArrayObject;

class ArrayObjectTest extends \PHPUnit_Framework_TestCase
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
