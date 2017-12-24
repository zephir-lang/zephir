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

class FibonnaciTest extends \PHPUnit_Framework_TestCase
{
    public function testMethods()
    {
        $t = new \Test\Fibonnaci();

        $this->assertSame($t->fibInt(), 13);
        $this->assertSame($t->fibDouble(), 13.0);
        $this->assertSame($t->fibArray(), array(
            0, 1, 1, 2, 3, 5, 8, 13, 21, 34
        ));
        $this->assertSame($t->fibArray2(), array(
            0, 1, 1, 2, 3, 5, 8, 13, 21, 34
        ));
    }
}
