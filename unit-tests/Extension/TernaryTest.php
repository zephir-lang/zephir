<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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
        $this->assertEquals(101, $t->testTernary1());
        $this->assertEquals('foo', $t->testTernary2(true));
        $this->assertEquals('bar', $t->testTernary2(false));
        
        $this->assertEquals(3, $t->testTernaryAfterLetVariable());
        $this->assertEquals(array('', 'c', ''), $t->testTernaryWithPromotedTemporaryVariable());
    }

    public function testComplex()
    {
        $t = new \Test\Ternary();
        $this->assertSame(101, $t->testTernaryComplex1(array(), ""));
        $this->assertSame(106, $t->testTernaryComplex2(array(), ""));
        $this->assertSame("boolean", $t->testTernaryComplex3(""));
    }
}
