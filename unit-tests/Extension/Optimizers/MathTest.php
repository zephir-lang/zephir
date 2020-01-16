<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Test\Optimizers\ACos;
use Test\Optimizers\ASin;
use Test\Optimizers\Cos;
use Test\Optimizers\Ldexp;
use Test\Optimizers\Sin;
use Test\Optimizers\Sqrt;
use Test\Optimizers\Tan;

class MathTest extends TestCase
{
    public function testLdexp()
    {
        $t = new Ldexp();

        $this->assertSame(16.0, $t->testInt());
        $this->assertSame(16.0, $t->testVar());
        $this->assertSame(16.0, $t->testIntValue1());
        $this->assertSame(16.0, $t->testDoubleInt());
        $this->assertSame(16.0, $t->testDouble());
        $this->assertSame(524288.0, $t->testIntParameter(8, 16));
        $this->assertSame(524288.0, $t->testvarParameter(8, 16));
    }

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

    public function testACos()
    {
        $t = new ACos();

        $this->assertSame(acos(1), $t->testInt());
        $this->assertSame(acos(1), $t->testVar());

        $this->assertSame(acos(0), $t->testIntValue1());

        $this->assertSame(acos(1), $t->testIntParameter(1));
        $this->assertSame(acos(0), $t->testIntParameter(0));

        $this->assertSame(acos(1), $t->testVarParameter(1));
        $this->assertSame(acos(0), $t->testVarParameter(0));
    }

    public function testASin()
    {
        $t = new ASin();

        $this->assertSame(asin(1), $t->testInt());
        $this->assertSame(asin(1), $t->testVar());

        $this->assertSame(asin(0), $t->testIntValue1());

        $this->assertSame(asin(1), $t->testIntParameter(1));
        $this->assertSame(asin(0), $t->testIntParameter(0));

        $this->assertSame(asin(1), $t->testVarParameter(1));
        $this->assertSame(asin(0), $t->testVarParameter(0));
    }

    public function testTan()
    {
        $t = new Tan();

        $this->assertSame(tan(0), $t->testInt());
        $this->assertSame(tan(0), $t->testVar());

        $this->assertSame(tan(1), $t->testVar2());
        $this->assertSame(tan(1), $t->testIntValue1());
        $this->assertSame(tan(2), $t->testIntValue2());

        $this->assertSame(tan(0), $t->testIntParameter(0));
        $this->assertSame(tan(1), $t->testIntParameter(1));
        $this->assertSame(tan(2), $t->testIntParameter(2));
        $this->assertSame(tan(3), $t->testIntParameter(3));
        $this->assertSame(tan(4), $t->testIntParameter(4));

        $this->assertSame(tan(0), $t->testVarParameter(0));
        $this->assertSame(tan(1), $t->testVarParameter(1));
        $this->assertSame(tan(2), $t->testVarParameter(2));
        $this->assertSame(tan(3), $t->testVarParameter(3));
        $this->assertSame(tan(4), $t->testVarParameter(4));
        $this->assertSame(tan(0), $t->testVarParameter('0'));
    }
}
