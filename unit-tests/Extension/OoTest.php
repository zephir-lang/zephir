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

class OoTest extends \PHPUnit_Framework_TestCase
{
    public function testAssertations()
    {
        $t = new \Test\Oo();

        $this->assertTrue($t->testInstance1() == new \stdClass);
        $this->assertTrue($t->testInstance2() == new \Test\Oo\OoConstruct);
        $this->assertTrue($t->testInstance3() == new \Test\Oo\OoNoConstruct);

        $obj4 = $t->testInstance4();
        $this->assertTrue(is_object($obj4));
        $this->assertTrue($obj4->getA() == "a");
        $this->assertTrue($obj4->getB() == "b");

        $obj5 = $t->testInstance5();
        $this->assertTrue(is_object($obj5));
        $this->assertTrue($obj5->getA() == "a");
        $this->assertTrue($obj5->getB() == "b");

        $obj6 = $t->testInstance6();
        $this->assertTrue(is_object($obj6));
        $this->assertTrue($obj6->getA() == 1);
        $this->assertTrue($obj6->getB() == 2);

        $obj7 = $t->testInstance7();
        $this->assertTrue(is_object($obj7));
        $this->assertTrue($obj7->getA() == false);
        $this->assertTrue($obj7->getB() == true);

        $obj8 = $t->testInstance8();
        $this->assertTrue(is_object($obj8));
        $this->assertTrue($obj8->getA() == 1.2);
        $this->assertTrue($obj8->getB() == 7.30);

        $obj9 = $t->testInstance9();
        $this->assertTrue(is_object($obj9));
        $this->assertTrue($obj9 instanceof \Test\Oo\OoDynamicA);

        $obj10 = $t->testInstance10();
        $this->assertTrue(is_object($obj10));
        $this->assertTrue($obj10 instanceof \Test\Oo\OoDynamicB);

        $obj = $t->testInstance11();
        $this->assertTrue(is_object($obj));
        $this->assertTrue($obj instanceof \Test\Oo\OoConstructParams);
    }
}
