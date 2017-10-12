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

use Test\Constants;
use Test\Oo\ConstantsInterface;

class ConstantsTest extends \PHPUnit_Framework_TestCase
{
    public function testConstantsDeclaration()
    {
        $t = new Constants();

        $this->assertNull(Constants::C1);
        $this->assertFalse(Constants::C2);
        $this->assertTrue(Constants::C3);
        $this->assertSame(Constants::C4, 10);
        $this->assertSame(Constants::C5, 10.25);
        $this->assertSame(Constants::C6, 'test');
        $this->assertSame(Constants::className, 'Test\Constants');
        $this->assertSame(Constants::STD_PROP_LIST, \ArrayObject::STD_PROP_LIST);
    }

    public function testConstantGetters()
    {
        $t = new Constants();
        $this->assertNull($t->getPropertyC1());
        $this->assertFalse($t->getPropertyC2());
        $this->assertTrue($t->getPropertyC3());
        $this->assertSame($t->getPropertyC4(), 10);
        $this->assertSame($t->getPropertyC5(), 10.25);
        $this->assertSame($t->getPropertyC6(), "test");
        $this->assertSame($t->getPropertyC9(), "some-value");
    }

    public function testConstantsRead()
    {
        $t = new Constants();
        $this->assertSame($t->testReadConstant(), ENT_QUOTES);
        $this->assertSame($t->testReadClassConstant1(), Constants::C4);
        $this->assertSame($t->testReadClassConstant2(), Constants::C4);
        $this->assertSame($t->testReadClassConstant3(), \Test\ConstantsParent::P4);
    }

    public function testEnvConstants()
    {
        $t = new Constants();

        $phpVersion = $t->testPHPVersionEnvConstant();
        $this->assertTrue(is_string($phpVersion) && !empty($phpVersion));

        $phpVersion = $t->testPHPVersionEnvConstantInExpValue();
        $this->assertTrue(is_string($phpVersion) && !empty($phpVersion));
    }

    public function testMagickConstants()
    {
        $t = new Constants();

        $this->assertSame($t->testNamespaceMagicConstant(), 'Test');
        $this->assertSame($t->testClassMagicConstant(), 'Test\Constants');

        $this->assertSame($t->testMethodMagicConstant(), 'Constants:testMethodMagicConstant');
        $this->assertSame($t->testFunctionMagicConstant(), 'testFunctionMagicConstant');


        $this->assertSame(ConstantsInterface::CLASSNAME, 'Test\Oo\ConstantsInterface');
        $this->assertSame(ConstantsInterface::NAMESPACENAME, 'Test\Oo');
    }
}
