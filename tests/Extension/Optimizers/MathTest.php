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
use Stub\Optimizers\ACos;
use Stub\Optimizers\ASin;
use Stub\Optimizers\Cos;
use Stub\Optimizers\Ldexp;
use Stub\Optimizers\Sin;
use Stub\Optimizers\Sqrt;
use Stub\Optimizers\Tan;

class MathTest extends TestCase
{
    public function testLdexp()
    {
        $test = new Ldexp();

        $this->assertSame(16.0, $test->testInt());
        $this->assertSame(16.0, $test->testVar());
        $this->assertSame(16.0, $test->testIntValue1());
        $this->assertSame(16.0, $test->testDoubleInt());
        $this->assertSame(16.0, $test->testDouble());
        $this->assertSame(524288.0, $test->testIntParameter(8, 16));
        $this->assertSame(524288.0, $test->testvarParameter(8, 16));
    }

    public function testSqrt()
    {
        $test = new Sqrt();

        $this->assertSame(2.0, $test->testInt());
        $this->assertSame(2.0, $test->testVar());

        $this->assertSame(2.0, $test->testIntValue1());
        $this->assertSame(4.0, $test->testIntValue2());

        $this->assertSame(2.0, $test->testIntParameter(4));
        $this->assertSame(4.0, $test->testIntParameter(16));

        $this->assertSame(2.0, $test->testVarParameter(4));
        $this->assertSame(4.0, $test->testVarParameter(16));
    }

    public function testSin()
    {
        $test = new Sin();

        $this->assertSame(sin(4), $test->testInt());
        $this->assertSame(sin(4), $test->testVar());

        $this->assertSame(sin(4), $test->testIntValue1());
        $this->assertSame(sin(16), $test->testIntValue2());

        $this->assertSame(sin(4), $test->testIntParameter(4));
        $this->assertSame(sin(16), $test->testIntParameter(16));

        $this->assertSame(sin(4), $test->testVarParameter(4));
        $this->assertSame(sin(16), $test->testVarParameter(16));
    }

    public function testCos()
    {
        $test = new Cos();

        $this->assertSame(cos(4), $test->testInt());
        $this->assertSame(cos(4), $test->testVar());

        $this->assertSame(cos(4), $test->testIntValue1());
        $this->assertSame(cos(16), $test->testIntValue2());

        $this->assertSame(cos(4), $test->testIntParameter(4));
        $this->assertSame(cos(16), $test->testIntParameter(16));

        $this->assertSame(cos(4), $test->testVarParameter(4));
        $this->assertSame(cos(16), $test->testVarParameter(16));
    }

    public function testACos()
    {
        $test = new ACos();

        $this->assertSame(acos(1), $test->testInt());
        $this->assertSame(acos(1), $test->testVar());

        $this->assertSame(acos(0), $test->testIntValue1());

        $this->assertSame(acos(1), $test->testIntParameter(1));
        $this->assertSame(acos(0), $test->testIntParameter(0));

        $this->assertSame(acos(1), $test->testVarParameter(1));
        $this->assertSame(acos(0), $test->testVarParameter(0));
    }

    public function testASin()
    {
        $test = new ASin();

        $this->assertSame(asin(1), $test->testInt());
        $this->assertSame(asin(1), $test->testVar());

        $this->assertSame(asin(0), $test->testIntValue1());

        $this->assertSame(asin(1), $test->testIntParameter(1));
        $this->assertSame(asin(0), $test->testIntParameter(0));

        $this->assertSame(asin(1), $test->testVarParameter(1));
        $this->assertSame(asin(0), $test->testVarParameter(0));
    }

    public function testTan()
    {
        $test = new Tan();

        $this->assertSame(tan(0), $test->testInt());
        $this->assertSame(tan(0), $test->testVar());

        $this->assertSame(tan(1), $test->testVar2());
        $this->assertSame(tan(1), $test->testIntValue1());
        $this->assertSame(tan(2), $test->testIntValue2());

        $this->assertSame(tan(0), $test->testIntParameter(0));
        $this->assertSame(tan(1), $test->testIntParameter(1));
        $this->assertSame(tan(2), $test->testIntParameter(2));
        $this->assertSame(tan(3), $test->testIntParameter(3));
        $this->assertSame(tan(4), $test->testIntParameter(4));

        $this->assertSame(tan(0), $test->testVarParameter(0));
        $this->assertSame(tan(1), $test->testVarParameter(1));
        $this->assertSame(tan(2), $test->testVarParameter(2));
        $this->assertSame(tan(3), $test->testVarParameter(3));
        $this->assertSame(tan(4), $test->testVarParameter(4));
        $this->assertSame(tan(0), $test->testVarParameter('0'));
    }
}
