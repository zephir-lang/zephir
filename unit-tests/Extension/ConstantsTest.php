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

use Test\Constants;
use Test\Oo\ConstantsInterface;

class ConstantsTest extends \PHPUnit_Framework_TestCase
{
    public function testConstantsDeclaration()
    {
        $t = new Constants();

        $this->assertTrue(Constants::C1 === null);
        $this->assertTrue(Constants::C2 === false);
        $this->assertTrue(Constants::C3 === true);
        $this->assertTrue(Constants::C4 === 10);
        $this->assertTrue(Constants::C5 === 10.25);
        $this->assertTrue(Constants::C6 === 'test');
        $this->assertTrue(Constants::className === 'Test\Constants');
        $this->assertTrue(Constants::STD_PROP_LIST === \ArrayObject::STD_PROP_LIST);
    }

    public function testConstantGetters()
    {
        $t = new Constants();
        $this->assertTrue($t->getPropertyC1() === null);
        $this->assertTrue($t->getPropertyC2() === false);
        $this->assertTrue($t->getPropertyC3() === true);
        $this->assertTrue($t->getPropertyC4() === 10);
        $this->assertTrue($t->getPropertyC5() === 10.25);
        $this->assertTrue($t->getPropertyC6() === "test");
    }

    public function testConstantsRead()
    {
        $t = new Constants();
        $this->assertTrue($t->testReadConstant() === ENT_QUOTES);
        $this->assertTrue($t->testReadClassConstant1() === Constants::C4);
        $this->assertTrue($t->testReadClassConstant2() === Constants::C4);
        $this->assertTrue($t->testReadClassConstant3() === \Test\ConstantsParent::P4);
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
