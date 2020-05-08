<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class OperatorTest extends TestCase
{
    /** @var \Stub\Operator */
    private $test;

    public function setUp()
    {
        $this->test = new \Stub\Operator();
    }

    public function test404Issue()
    {
        $this->assertFalse($this->test->testIdentical(true, 1));
        $this->assertFalse($this->test->testIdentical(true, 'phalcon'));
        $this->assertFalse($this->test->testIdentical(false, 0));
        $this->assertFalse($this->test->testIdentical(1, '1'));
        $this->assertTrue($this->test->testIdentical(1, 1));
        $this->assertTrue($this->test->testIdentical(true, true));
        $this->assertTrue($this->test->testIdentical('phalcon', 'phalcon'));
    }

    public function test829Issue()
    {
        $this->assertTrue($this->test->testIdenticalVarFalse(false));
        $this->assertFalse($this->test->testIdenticalVarFalse(0));
        $this->assertFalse($this->test->testIdenticalVarFalse(''));
        $this->assertTrue($this->test->testIdenticalFalseVar(false));
        $this->assertFalse($this->test->testIdenticalFalseVar(0));
        $this->assertFalse($this->test->testIdenticalFalseVar(''));
    }

    public function test1072Issue()
    {
        $this->assertFalse($this->test->testIdenticalIfComplex(false));
        $this->assertTrue($this->test->testIdenticalIfComplex(0));
        $this->assertTrue($this->test->testIdenticalIfComplex(''));
    }

    public function test1279Issue()
    {
        $values = $this->test->testBoolComparison(true, false);

        $this->assertTrue($values[0]);
        $this->assertTrue($values[1]);
        $this->assertTrue($values[2]);
        $this->assertTrue($values[3]);
        $this->assertTrue($values[4]);
        $this->assertTrue($values[5]);
        $this->assertTrue($values[6]);
        $this->assertTrue($values[7]);
    }
}
