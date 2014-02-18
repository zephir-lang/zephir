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

class TernaryTest extends \PHPUnit_Framework_TestCase
{
    public function testTernary()
    {
        $t = new \Test\Ternary();
        $this->assertTrue($t->testTernary1() === 101);
        $this->assertTrue($t->testTernary2(true) === "foo");
        $this->assertTrue($t->testTernary2(false) === "bar");
    }

    public function testComplex()
    {
        $t = new \Test\Ternary();
        $this->assertTrue($t->testTernaryComplex1(array(), "") === 101);
        $this->assertTrue($t->testTernaryComplex2(array(), "") === 106);
        $this->assertTrue($t->testTernaryComplex3("") === "boolean");
    }
}
