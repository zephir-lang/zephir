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

namespace Extension\BuiltIn;

use Test\BuiltIn\IntMethods;

class IntMethodTest extends \PHPUnit_Framework_TestCase
{
    public function testMath()
    {
        $t = new IntMethods();

        $this->assertSame(abs(10), $t->getAbs(10));
        $this->assertSame(abs(-5), $t->getAbs1());
        $this->assertSame(pow(2, 4), $t->getPow(2, 4));
        $this->assertSame(sqrt(4), $t->getSqrt(4));
        $this->assertSame(exp(2), $t->getExp(2));

        $this->assertSame(log(10), $t->getLog(10));
        $this->assertSame(log(10, 10), $t->getLog(10, 10));
    }

    public function testTrigonometry()
    {
        $t = new IntMethods();

        $this->assertSame(sin(1), $t->getSin(1));
        $this->assertSame(cos(1), $t->getCos(1));
        $this->assertSame(tan(1), $t->getTan(1));
        $this->assertSame(asin(1), $t->getAsin(1));
        $this->assertSame(acos(1), $t->getAcos(1));
        $this->assertSame(atan(1), $t->getAtan(1));
    }

    public function testTransforms()
    {
        $t = new IntMethods();

        $this->assertSame(decbin(255), $t->getBinary(255));
        $this->assertSame(dechex(255), $t->getHex(255));
        $this->assertSame(decoct(255), $t->getOctal(255));
    }
}
