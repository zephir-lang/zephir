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

class ScallTest extends \PHPUnit_Framework_TestCase
{
    public function testScall()
    {
        $t = new \Test\Scall();

        $this->assertTrue($t->testMethod1() === "hello public");
        $this->assertTrue($t::testMethod1() === "hello public");
        $this->assertTrue(\Test\Scall::testMethod1() === "hello public");

        $this->assertTrue($t->testCall1() === "hello public");
        $this->assertTrue($t->testCall2() === "hello protected");
        $this->assertTrue($t->testCall3() === "hello private");

        $this->assertTrue($t->testCall4(2, 3) === 5);
        $this->assertTrue($t->testCall5(6, 7) === 13);
        $this->assertTrue($t->testCall6(4, 5) === 9);

        $this->assertTrue($t->testCall7() === "hello public");
        $this->assertTrue($t->testCall8() === "hello protected");
        $this->assertTrue($t->testCall9() === "hello private");

        $this->assertTrue($t->testCall10(2, 3) === 5);
        $this->assertTrue($t->testCall11(6, 7) === 13);
        $this->assertTrue($t->testCall12(4, 5) === 9);

        $this->assertTrue($t->testCall13() == "hello parent public");
        $this->assertTrue($t->testCall14() == "hello parent protected");

        $this->assertEquals("hello ScallParent", \Test\Scallparent::testCallStatic());
        $this->assertEquals("hello Scall", $t::testCallStatic());
    }
}
