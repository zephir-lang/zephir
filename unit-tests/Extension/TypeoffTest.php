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

namespace Extension;

class TypeoffTest extends \PHPUnit_Framework_TestCase
{
    public function testNativeString()
    {
        $t = new \Test\Typeoff();
        $this->assertTrue($t->testNativeStringFalse() == false);
        $this->assertTrue($t->testNativeStringTrue() == true);
    }

    public function testNativeInt()
    {
        $t = new \Test\Typeoff();
        $this->assertTrue($t->testNativeIntFalse() == false);
        $this->assertTrue($t->testNativeIntTrue() == true);
    }

    public function testNativeDouble()
    {
        $t = new \Test\Typeoff();
        $this->assertTrue($t->testNativeDoubleTrue() == true);
    }

    public function testNativeBool()
    {
        $t = new \Test\Typeoff();
        $this->assertTrue($t->testNativeBoolFalse() == false);
        $this->assertTrue($t->testNativeBoolTrue() == true);
    }

    public function testArray()
    {
        $t = new \Test\Typeoff();
        $this->assertTrue($t->testArrayFalse() == false);
        $this->assertTrue($t->testArrayTrue() == true);
    }

    public function testClassProperty()
    {
        $t = new \Test\Typeoff();
        $this->assertTrue($t->testClassPropertyAccess() == true);
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
