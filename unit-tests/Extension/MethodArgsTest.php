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

use Test\MethodArgs;

class MethodArgsTest extends \PHPUnit_Framework_TestCase
{
    public function testCallable()
    {
        $t = new MethodArgs;
        $callback = function () {
        };
        $t->setCallable($callback);

        $this->assertSame($callback, $t->a);

        $t->setCallableStrict($callback);

        $this->assertSame($callback, $t->a);

        $this->setExpectedException('\Exception');
        $t->setCallableStrict(true);
    }

    public function testObject()
    {
        $t = new MethodArgs;
        $obj = new \stdClass;

        $t->setObject($obj);

        $this->assertSame($obj, $t->a);

        $t->setObjectStrict($obj);

        $this->assertSame($obj, $t->a);

        $this->setExpectedException('\Exception');
        $t->setObjectStrict(true);
    }

    public function testResource()
    {
        $t = new MethodArgs;
        $t->setResourceStrict(STDIN);

        $this->assertSame(STDIN, $t->a);

        $this->setExpectedException('\Exception');
        $t->setResourceStrict(true);
    }

    public function testMethodOptionalValueWithDefaultStaticConstantAccess()
    {
        $t = new MethodArgs;
        $this->assertEquals('test', $t->methodOptionalValueWithDefaultStaticConstantAccess('test'));

        $this->assertEquals(MethodArgs::GET, $t->methodOptionalValueWithDefaultStaticConstantAccess());
        $this->assertEquals(MethodArgs::GET, $t->methodOptionalStringValueWithDefaultStaticConstantAccess());
        $this->assertEquals(MethodArgs::MY_DOUBLE, $t->methodOptionalDoubleValueWithDefaultStaticConstantAccess());
        $this->assertEquals(MethodArgs::MY_BOOL, $t->methodOptionalBoolValueWithDefaultStaticConstantAccess());
        $this->assertEquals(MethodArgs::MY_INT, $t->methodOptionalIntValueWithDefaultStaticConstantAccess());
    }
}
