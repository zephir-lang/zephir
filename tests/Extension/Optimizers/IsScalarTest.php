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
use Stub\Optimizers\IsScalar;

class IsScalarTest extends TestCase
{
    /** @var IsScalar */
    private $test;

    public function setUp()
    {
        $this->test = new IsScalar();
    }

    public function testVariable()
    {
        $this->assertTrue($this->test->testIntVar());
        $this->assertTrue($this->test->testDoubleVar());
        $this->assertTrue($this->test->testBoolVar());
        $this->assertTrue($this->test->testStringVar());
        $this->assertTrue($this->test->testVar());

        $this->assertFalse($this->test->testEmptyArrayVar());
    }

    public function testVariableParameter()
    {
        $this->assertTrue($this->test->testVarParameter(1));
        $this->assertTrue($this->test->testVarParameter(1.0));
        $this->assertTrue($this->test->testVarParameter(true));
        $this->assertTrue($this->test->testVarParameter(false));
        $this->assertTrue($this->test->testVarParameter(''));
        $this->assertTrue($this->test->testVarParameter('test string'));

        $this->assertFalse($this->test->testVarParameter([]));
        $this->assertFalse($this->test->testVarParameter([1, 2, 3, 4, 5]));
        $this->assertFalse($this->test->testVarParameter($this));
        $this->assertFalse(
            $this->test->testVarParameter(
                function () {
                }
            )
        );
        $this->assertFalse($this->test->testVarParameter(null));
    }
}
