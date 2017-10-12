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

use Test\Compare;

class CompareTest extends \PHPUnit_Framework_TestCase
{
    public function testLess()
    {
        $t = new Compare;

        $this->assertTrue($t->isLessInt(1, 2));
        $this->assertTrue($t->isLessDouble(1.1, 1.2));
        $this->assertFalse($t->isLessInt(20, 10));
    }

    public function testGreaterEqual()
    {
        $t = new Compare;

        $this->assertTrue($t->isGreaterEqual(3, 2));
        $this->assertTrue($t->isGreaterEqual(2, 2));
        $this->assertFalse($t->isGreaterEqual(1, 2));
    }

    public function testConst()
    {
        $t = new Compare;

        $this->assertTrue($t->isLessThenPi(3.12));
        $this->assertTrue($t->isMoreThenPi(3.15));
    }

    public function testVarWithStringEquals()
    {
        $t = new Compare;

        $this->assertSame('NOK', $t->testVarWithStringEquals('wrong testing'));
        $this->assertSame('NOK', $t->testVarWithStringEquals('another testing'));
        $this->assertSame('OK', $t->testVarWithStringEquals('testing'));
        $this->assertSame('NOK', $t->testVarWithStringEquals('testing nok'));
    }

    public function testEquals()
    {
        $t = new Compare();

        $this->assertTrue($t->testVarEqualsNull(null));
        $this->assertFalse($t->testVarEqualsNull(1));

        $this->assertTrue($t->testNullEqualsVar(null));
        $this->assertFalse($t->testNullEqualsVar(1));
    }


    public function testNotEquals()
    {
        $t = new Compare();

        $this->assertTrue($t->testNotIdenticalZeroVar());
        $this->assertTrue($t->testNotIdenticalZeroInt());
        $this->assertTrue($t->testNotIdenticalZeroLong());
    }
}
