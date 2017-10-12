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

        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\Exception');
        } else {
            $this->expectException('\Exception');
        }

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

        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\Exception');
        } else {
            $this->expectException('\Exception');
        }

        $t->setObjectStrict(true);
    }

    public function testResource()
    {
        $t = new MethodArgs;
        $t->setResourceStrict(STDIN);

        $this->assertSame(STDIN, $t->a);

        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\Exception');
        } else {
            $this->expectException('\Exception');
        }

        $t->setResourceStrict(true);
    }

    public function testMethodOptionalValueWithDefaultStaticConstantAccess()
    {
        $t = new MethodArgs;
        $this->assertSame('test', $t->methodOptionalValueWithDefaultStaticConstantAccess('test'));

        $this->assertSame(MethodArgs::GET, $t->methodOptionalValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::GET, $t->methodOptionalStringValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_DOUBLE, $t->methodOptionalDoubleValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_BOOL, $t->methodOptionalBoolValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_INT, $t->methodOptionalIntValueWithDefaultStaticConstantAccess());
    }
}
