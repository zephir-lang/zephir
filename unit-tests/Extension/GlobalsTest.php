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

class GlobalsTest extends \PHPUnit_Framework_TestCase
{
    public function testGlobals()
    {
        $t = new \Test\Globals();

        $this->assertTrue($t->getDefaultGlobals1());
        $this->assertSame($t->getDefaultGlobals2(), 100);
        $this->assertSame($t->getDefaultGlobals3(), 7.5);
        $this->assertTrue($t->getDefaultGlobals4());
        $this->assertSame($t->getDefaultGlobals5(), 10);
        $this->assertSame($t->getDefaultGlobals6(), 15.2);
    }
}
