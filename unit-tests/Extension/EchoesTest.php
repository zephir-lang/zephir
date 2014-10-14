<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

namespace Extension;

class EchoesTest extends \PHPUnit_Framework_TestCase
{
    public function testEchoInternal($target = null, $method = null, $expect = null)
    {
        if (!$target) {
            return $this->assertTrue(true);
        }
        ob_start();
        call_user_func(array($target, $method));
        $str = ob_get_clean();
        $this->assertTrue($str == $expect, $method."s return did not meet expectations!");
    }

    public function testEcho()
    {
        $cls = new \Test\Echoes();
        $this->testEchoInternal($cls, "testEcho1", "1");
        $this->testEchoInternal($cls, "testEcho2", "1");
        $this->testEchoInternal($cls, "testEcho3", "1");
        $this->testEchoInternal($cls, "testEcho4", "");
        $this->testEchoInternal($cls, "testEcho5", "hello");
        $this->testEchoInternal($cls, "testEcho6", 'hello here is a quote: "');
        $this->testEchoInternal($cls, "testEcho7", "\r\t echo' \"");
    }
}
