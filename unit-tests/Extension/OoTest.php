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

class OoTest extends \PHPUnit_Framework_TestCase
{
    public function testAssertations()
    {
        $t = new \Test\Oo();

        $this->assertEquals($t->testInstance1(), new \stdClass);
        $this->assertEquals($t->testInstance2(), new \Test\Oo\OoConstruct);
        $this->assertEquals($t->testInstance3(), new \Test\Oo\OoNoConstruct);

        $obj4 = $t->testInstance4();
        $this->assertTrue(is_object($obj4));
        $this->assertSame($obj4->getA(), "a");
        $this->assertSame($obj4->getB(), "b");

        $obj5 = $t->testInstance5();
        $this->assertTrue(is_object($obj5));
        $this->assertSame($obj5->getA(), "a");
        $this->assertSame($obj5->getB(), "b");

        $obj6 = $t->testInstance6();
        $this->assertTrue(is_object($obj6));
        $this->assertSame($obj6->getA(), 1);
        $this->assertSame($obj6->getB(), 2);

        $obj7 = $t->testInstance7();
        $this->assertTrue(is_object($obj7));
        $this->assertSame($obj7->getA(), false);
        $this->assertSame($obj7->getB(), true);

        $obj8 = $t->testInstance8();
        $this->assertTrue(is_object($obj8));
        $this->assertSame($obj8->getA(), 1.2);
        $this->assertSame($obj8->getB(), 7.30);

        $obj9 = $t->testInstance9();
        $this->assertTrue(is_object($obj9));
        $this->assertInstanceOf('Test\Oo\OoDynamicA', $obj9);

        $obj10 = $t->testInstance10();
        $this->assertTrue(is_object($obj10));
        $this->assertInstanceOf('Test\Oo\OoDynamicB', $obj10);

        $obj = $t->testInstance11();
        $this->assertTrue(is_object($obj));
        $this->assertInstanceOf('Test\Oo\OoConstructParams', $obj);

        $obj12 = $t->testInstance12();
        $this->assertTrue(is_object($obj12));
        $this->assertInstanceOf('Test\Oo\OoDynamicA', $obj12);
    }
}
