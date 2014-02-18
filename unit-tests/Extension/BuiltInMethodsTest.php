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

class BuiltInMethodsTest extends \PHPUnit_Framework_TestCase
{
    public function testStringMethodLength()
    {
        $t = new \Test\BuiltInMethods();
        $this->assertTrue($t->stringMethodLength1() == 5);
        $this->assertTrue($t->stringMethodLength2() == 5);
        $this->assertTrue($t->stringMethodLength3() == 10);
        $this->assertTrue($t->stringMethodLength4("hello") == 5);
        $this->assertTrue($t->stringMethodLength5("hello") == 10);
    }

    public function testStringTrim()
    {
        $t = new \Test\BuiltInMethods();
        $this->assertTrue($t->stringMethodTrim1() == "hello");
    }

    /**
    public function testIntAbs()
    {
        $t = new \Test\BuiltInMethods();
        $this->assertTrue($t->intMethodAbs1() == 100);
        $this->assertTrue($t->intMethodAbs2() == 50);
    }*/
}
