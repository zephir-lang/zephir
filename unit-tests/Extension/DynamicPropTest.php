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

class DynamicPropTest extends \PHPUnit_Framework_TestCase
{
    public function testSetProperty()
    {
        $class = new \Test\Oo\DynamicProp();
        $this->assertEquals(null, $class->test);

        $class->setPropertyInt('test');
        $this->assertEquals(10, $class->test);

        $class->setPropertyBoolTrue('test');
        $this->assertEquals(true, $class->test);

        $class->setPropertyBoolFalse('test');
        $this->assertEquals(false, $class->test);

        $class->setPropertyString('test');
        $this->assertEquals('string', $class->test);

        $class->setPropertyChar('test');
        $this->assertEquals(ord('A'), $class->test);

        $class->setPropertyUChar('test');
        $this->assertEquals(ord('B')-1, $class->test);

        $class->setPropertyNull('test');
        $this->assertEquals(null, $class->test);


        $class->setPropertyVariableInt('test');
        $this->assertEquals(10, $class->test);

        $class->setPropertyVariableString('test');
        $this->assertEquals("test", $class->test);

        $class->setPropertyVariableBoolTrue('test');
        $this->assertEquals(true, $class->test);

        $class->setPropertyVariableBoolFalse('test');
        $this->assertEquals(false, $class->test);

        $class->setPropertyInt("property");
        $this->assertEquals(10, $class->property);

        $class->setExistingStringProperty();
        $this->assertEquals("works", $class->test);

        $class->setExistingStringPropertyString("test");
        $this->assertEquals("test", $class->eproperty);

        $class->setNonExistingStringProperty();
        $this->assertEquals("works", $class->property);
    }
}
