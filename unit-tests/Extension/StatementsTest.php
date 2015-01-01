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

use Test\Statements;

class StatementsTest extends \PHPUnit_Framework_TestCase
{
    public function testElseIf()
    {
        $t = new Statements;
        $this->assertEquals('more', $t->testElseIf(10));
        $this->assertEquals('equal', $t->testElseIf(0));
        $this->assertEquals('-1', $t->testElseIf(-1));
        $this->assertEquals('less', $t->testElseIf(-10));
    }

    public function testElseIf1()
    {
        $t = new Statements;
        $this->assertEquals('equal', $t->testElseIf1(10));
        $this->assertEquals('less', $t->testElseIf1(5));
        $this->assertEquals('else', $t->testElseIf1(11));
    }

    public function testElseIf2()
    {
        $t = new Statements;
        $this->assertEquals('equal', $t->testElseIf2(10, 10));
        $this->assertEquals('less', $t->testElseIf2(5, 10));
        $this->assertEquals('else', $t->testElseIf2(11, 10));
    }

    public function test544Issue()
    {
        $t = new Statements;

        $this->assertSame('====>...............................................................................................', $t->test544Issue(5));
        $this->assertSame('====================================================================================================', $t->test544Issue(100));

        $this->assertSame('====>...............................................................................................', $t->test544IssueWithVariable(5));
        $this->assertSame('====================================================================================================', $t->test544IssueWithVariable(100));
    }
}
