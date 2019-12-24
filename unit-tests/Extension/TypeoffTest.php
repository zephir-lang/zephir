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
        $this->assertSame('array', $t->testUnknownTypeOf([]));
        $this->assertSame('object', $t->testUnknownTypeOf(new \stdClass()));
        $this->assertSame('NULL', $t->testUnknownTypeOf(null));
    }
}
