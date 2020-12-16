<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\BuiltIn;

use PHPUnit\Framework\TestCase;
use Stub\BuiltIn\IntMethods;

class IntMethodTest extends TestCase
{
    /** @var IntMethods */
    private $test;

    protected function setUp(): void
    {
        $this->test = new IntMethods();
    }

    public function testMath()
    {
        $this->assertSame(abs(10), $this->test->getAbs(10));
        $this->assertSame(abs(-5), $this->test->getAbs1());
        $this->assertSame(2 ** 4, $this->test->getPow(2, 4));
        $this->assertSame(sqrt(4), $this->test->getSqrt(4));
        $this->assertSame(exp(2), $this->test->getExp(2));

        $this->assertSame(log(10), $this->test->getLog(10));
        $this->assertSame(log(10, 10), $this->test->getLog(10, 10));
    }

    public function testTrigonometry()
    {
        $this->assertSame(sin(1), $this->test->getSin(1));
        $this->assertSame(cos(1), $this->test->getCos(1));
        $this->assertSame(tan(1), $this->test->getTan(1));
        $this->assertSame(asin(1), $this->test->getAsin(1));
        $this->assertSame(acos(1), $this->test->getAcos(1));
        $this->assertSame(atan(1), $this->test->getAtan(1));
    }

    public function testTransforms()
    {
        $this->assertSame(decbin(255), $this->test->getBinary(255));
        $this->assertSame(dechex(255), $this->test->getHex(255));
        $this->assertSame(decoct(255), $this->test->getOctal(255));
    }
}
