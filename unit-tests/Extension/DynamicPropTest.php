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

class DynamicPropTest extends \PHPUnit_Framework_TestCase
{
    public function testSetProperty()
    {
        $class = new \Test\Oo\DynamicProp();
        $this->assertSame(null, $class->test);

        $class->setPropertyInt('test');
        $this->assertSame(10, $class->test);

        $class->setPropertyBoolTrue('test');
        $this->assertSame(true, $class->test);

        $class->setPropertyBoolFalse('test');
        $this->assertSame(false, $class->test);

        $class->setPropertyString('test');
        $this->assertSame('string', $class->test);

        $class->setPropertyChar('test');
        $this->assertSame(ord('A'), $class->test);

        $class->setPropertyUChar('test');
        $this->assertSame(ord('B')-1, $class->test);

        $class->setPropertyNull('test');
        $this->assertSame(null, $class->test);


        $class->setPropertyVariableInt('test');
        $this->assertSame(10, $class->test);

        $class->setPropertyVariableString('test');
        $this->assertSame("test", $class->test);

        $class->setPropertyVariableBoolTrue('test');
        $this->assertSame(true, $class->test);

        $class->setPropertyVariableBoolFalse('test');
        $this->assertSame(false, $class->test);

        $class->setPropertyInt("property");
        $this->assertSame(10, $class->property);

        $class->setExistingStringProperty();
        $this->assertSame("works", $class->test);

        $class->setExistingStringPropertyString("test");
        $this->assertSame("test", $class->eproperty);

        $class->setNonExistingStringProperty();
        $this->assertSame("works", $class->property);
    }
}
