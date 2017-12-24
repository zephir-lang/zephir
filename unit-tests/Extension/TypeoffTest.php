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

class TypeoffTest extends \PHPUnit_Framework_TestCase
{
    public function testNativeString()
    {
        $t = new \Test\Typeoff();
        $this->assertFalse($t->testNativeStringFalse());
        $this->assertTrue($t->testNativeStringTrue());
    }

    public function testNativeInt()
    {
        $t = new \Test\Typeoff();
        $this->assertFalse($t->testNativeIntFalse());
        $this->assertTrue($t->testNativeIntTrue());
    }

    public function testNativeDouble()
    {
        $t = new \Test\Typeoff();
        $this->assertTrue($t->testNativeDoubleTrue());
    }

    public function testNativeBool()
    {
        $t = new \Test\Typeoff();
        $this->assertFalse($t->testNativeBoolFalse());
        $this->assertTrue($t->testNativeBoolTrue());
        $this->assertFalse($t->testNotBoolTrue());
    }

    public function testArray()
    {
        $t = new \Test\Typeoff();
        $this->assertFalse($t->testArrayFalse());
        $this->assertTrue($t->testArrayTrue());
    }

    public function testClassProperty()
    {
        $t = new \Test\Typeoff();
        $this->assertTrue($t->testClassPropertyAccess());
    }

    public function testCallable()
    {
        $t = new \Test\Typeoff();
        $callback = function () {
        };
        $this->assertTrue($t->testCallableTypeOf($callback));
        $this->assertFalse($t->testCallableTypeOf(true));
    }

    public function testUnknown()
    {
        $t = new \Test\Typeoff();

        $this->assertSame('string', $t->testUnknownTypeOf('test'));
        $this->assertSame('integer', $t->testUnknownTypeOf(5));
        $this->assertSame('boolean', $t->testUnknownTypeOf(true));
        $this->assertSame('double', $t->testUnknownTypeOf(5.5));
        $this->assertSame('array', $t->testUnknownTypeOf(array()));
        $this->assertSame('object', $t->testUnknownTypeOf(new \stdClass));
        $this->assertSame('NULL', $t->testUnknownTypeOf(null));
    }
}
