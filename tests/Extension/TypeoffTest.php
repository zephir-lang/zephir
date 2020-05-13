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

class TypeoffTest extends TestCase
{
    /** @var \Stub\Typeoff */
    private $test;

    public function setUp()
    {
        $this->test = new \Stub\Typeoff();
    }

    public function testNativeString()
    {
        $this->assertFalse($this->test->testNativeStringFalse());
        $this->assertTrue($this->test->testNativeStringTrue());
    }

    public function testNativeInt()
    {
        $this->assertFalse($this->test->testNativeIntFalse());
        $this->assertTrue($this->test->testNativeIntTrue());
    }

    public function testNativeDouble()
    {
        $this->assertTrue($this->test->testNativeDoubleTrue());
    }

    public function testNativeBool()
    {
        $this->assertFalse($this->test->testNativeBoolFalse());
        $this->assertTrue($this->test->testNativeBoolTrue());
        $this->assertFalse($this->test->testNotBoolTrue());
    }

    public function testArray()
    {
        $this->assertFalse($this->test->testArrayFalse());
        $this->assertTrue($this->test->testArrayTrue());
    }

    public function testClassProperty()
    {
        $this->assertTrue($this->test->testClassPropertyAccess());
    }

    public function testCallable()
    {
        $callback = function () {
        };
        $this->assertTrue($this->test->testCallableTypeOf($callback));
        $this->assertFalse($this->test->testCallableTypeOf(true));
    }

    public function testUnknown()
    {
        $this->assertSame('string', $this->test->testUnknownTypeOf('test'));
        $this->assertSame('integer', $this->test->testUnknownTypeOf(5));
        $this->assertSame('boolean', $this->test->testUnknownTypeOf(true));
        $this->assertSame('double', $this->test->testUnknownTypeOf(5.5));
        $this->assertSame('array', $this->test->testUnknownTypeOf([]));
        $this->assertSame('object', $this->test->testUnknownTypeOf(new \stdClass()));
        $this->assertSame('NULL', $this->test->testUnknownTypeOf(null));
    }
}
