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

namespace Extension\BuiltIn;

use Test\BuiltIn\IntMethods;

class IntMethodTest extends \PHPUnit_Framework_TestCase
{
    public function testMath()
    {
        $t = new IntMethods();

        $this->assertEquals(abs(10), $t->getAbs(10));
        $this->assertEquals(abs(-5), $t->getAbs1());
        $this->assertEquals(pow(2, 4), $t->getPow(2, 4));
        $this->assertEquals(sqrt(4), $t->getSqrt(4));
        $this->assertEquals(exp(2), $t->getExp(2));

        $this->assertEquals(log(10), $t->getLog(10));
        $this->assertEquals(log(10, 10), $t->getLog(10, 10));
    }

    public function testTrigonometry()
    {
        $t = new IntMethods();

        $this->assertEquals(sin(1), $t->getSin(1));
        $this->assertEquals(cos(1), $t->getCos(1));
        $this->assertEquals(tan(1), $t->getTan(1));
        $this->assertEquals(asin(1), $t->getAsin(1));
        $this->assertEquals(acos(1), $t->getAcos(1));
        $this->assertEquals(atan(1), $t->getAtan(1));
    }

    public function testTransforms()
    {
        $t = new IntMethods();

        $this->assertEquals(decbin(255), $t->getBinary(255));
        $this->assertEquals(dechex(255), $t->getHex(255));
        $this->assertEquals(decoct(255), $t->getOctal(255));
    }
}
