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

class ScallExternalTest extends \PHPUnit_Framework_TestCase
{
    public function testCall1()
    {
        $t = new \Test\ScallExternal();
        $this->assertSame($t->testCall1(), 'hello public');
    }

    public function testCall2()
    {
        $t = new \Test\ScallExternal();
        $this->assertSame($t->testCall2(2, 3), 5);
    }
}
