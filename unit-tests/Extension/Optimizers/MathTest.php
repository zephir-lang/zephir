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

namespace Extension\Optimizers;

use Test\Optimizers\Sqrt;
use Test\Optimizers\Sin;
use Test\Optimizers\Cos;

class MathTest extends \PHPUnit_Framework_TestCase
{
    public function testSqrt()
    {
        $t = new Sqrt();

        $this->assertSame(2.0, $t->testInt());
        $this->assertSame(2.0, $t->testVar());

        $this->assertSame(2.0, $t->testIntValue1());
        $this->assertSame(4.0, $t->testIntValue2());

        $this->assertSame(2.0, $t->testIntParameter(4));
        $this->assertSame(4.0, $t->testIntParameter(16));

        $this->assertSame(2.0, $t->testVarParameter(4));
        $this->assertSame(4.0, $t->testVarParameter(16));
    }

    public function testSin()
    {
        $t = new Sin();

        $this->assertSame(sin(4), $t->testInt());
        $this->assertSame(sin(4), $t->testVar());

        $this->assertSame(sin(4), $t->testIntValue1());
        $this->assertSame(sin(16), $t->testIntValue2());

        $this->assertSame(sin(4), $t->testIntParameter(4));
        $this->assertSame(sin(16), $t->testIntParameter(16));

        $this->assertSame(sin(4), $t->testVarParameter(4));
        $this->assertSame(sin(16), $t->testVarParameter(16));
    }

    public function testCos()
    {
        $t = new Cos();

        $this->assertSame(cos(4), $t->testInt());
        $this->assertSame(cos(4), $t->testVar());

        $this->assertSame(cos(4), $t->testIntValue1());
        $this->assertSame(cos(16), $t->testIntValue2());

        $this->assertSame(cos(4), $t->testIntParameter(4));
        $this->assertSame(cos(16), $t->testIntParameter(16));

        $this->assertSame(cos(4), $t->testVarParameter(4));
        $this->assertSame(cos(16), $t->testVarParameter(16));
    }
}
