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

class McallChainedTest extends \PHPUnit_Framework_TestCase
{
    public function testChained1()
    {
        $t = new \Test\McallChained();
        $this->assertSame($t->testChained1(), $t);
    }

    public function testChained2()
    {
        $t = new \Test\McallChained();
        $this->assertSame($t->testChained2(), $t);
    }

    public function testChained3()
    {
        $t = new \Test\McallChained();
        $this->assertSame($t->testChained3(), $t);
    }
}
