<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team and contributors                     |
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
        $this->assertSame(101, $t->testTernary1());
        $this->assertSame('foo', $t->testTernary2(true));
        $this->assertSame('bar', $t->testTernary2(false));

        $this->assertSame(3, $t->testTernaryAfterLetVariable());
        $this->assertSame(array('', 'c', ''), $t->testTernaryWithPromotedTemporaryVariable());

        $this->assertSame(true, $t->testShortTernary(true));
        $this->assertSame(false, $t->testShortTernary(array()));
        $this->assertSame(array(1,2,3), $t->testShortTernary(array(1,2,3)));
        $this->assertSame(false, $t->testShortTernary(false));
        $this->assertSame(false, $t->testShortTernary(0));

        $this->assertSame(1, $t->testShortTernaryComplex(false, 1));
        $this->assertSame("test string", $t->testShortTernaryComplex(false, "test string"));
        $this->assertSame(array(), $t->testShortTernaryComplex(false, array()));
    }

    public function testComplex()
    {
        $t = new \Test\Ternary();
        $this->assertSame(101, $t->testTernaryComplex1(array(), ""));
        $this->assertSame(106, $t->testTernaryComplex2(array(), ""));
        $this->assertSame("boolean", $t->testTernaryComplex3(""));
    }
}
