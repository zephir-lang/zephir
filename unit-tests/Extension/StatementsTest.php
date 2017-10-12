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

use Test\Statements;

class StatementsTest extends \PHPUnit_Framework_TestCase
{
    public function testElseIf()
    {
        $t = new Statements;
        $this->assertSame('more', $t->testElseIf(10));
        $this->assertSame('equal', $t->testElseIf(0));
        $this->assertSame('-1', $t->testElseIf(-1));
        $this->assertSame('less', $t->testElseIf(-10));
    }

    public function testElseIf1()
    {
        $t = new Statements;
        $this->assertSame('equal', $t->testElseIf1(10));
        $this->assertSame('less', $t->testElseIf1(5));
        $this->assertSame('else', $t->testElseIf1(11));
    }

    public function testElseIf2()
    {
        $t = new Statements;
        $this->assertSame('equal', $t->testElseIf2(10, 10));
        $this->assertSame('less', $t->testElseIf2(5, 10));
        $this->assertSame('else', $t->testElseIf2(11, 10));
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
