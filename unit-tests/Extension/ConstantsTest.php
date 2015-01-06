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

class ConstantsTest extends \PHPUnit_Framework_TestCase
{
    public function testConstantsDeclaration()
    {
        $t = new \Test\Constants();

        $this->assertTrue(\Test\Constants::C1 === null);
        $this->assertTrue(\Test\Constants::C2 === false);
        $this->assertTrue(\Test\Constants::C3 === true);
        $this->assertTrue(\Test\Constants::C4 === 10);
        $this->assertTrue(\Test\Constants::C5 === 10.25);
        $this->assertTrue(\Test\Constants::C6 === "test");

    }

    public function testConstantGetters()
    {
        $t = new \Test\Constants();
        $this->assertTrue($t->getPropertyC1() === null);
        $this->assertTrue($t->getPropertyC2() === false);
        $this->assertTrue($t->getPropertyC3() === true);
        $this->assertTrue($t->getPropertyC4() === 10);
        $this->assertTrue($t->getPropertyC5() === 10.25);
        $this->assertTrue($t->getPropertyC6() === "test");
    }

    public function testConstantsRead()
    {
        $t = new \Test\Constants();
        $this->assertTrue($t->testReadConstant() === ENT_QUOTES);
        $this->assertTrue($t->testReadClassConstant1() === \Test\Constants::C4);
        $this->assertTrue($t->testReadClassConstant2() === \Test\Constants::C4);
        $this->assertTrue($t->testReadClassConstant3() === \Test\ConstantsParent::P4);
    }

    public function testEnvConstants()
    {
        $t = new \Test\Constants();

        $phpVersion = $t->testPHPVersionEnvConstant();
        $this->assertTrue(is_string($phpVersion) && !empty($phpVersion));

        $phpVersion = $t->testPHPVersionEnvConstantInExpValue();
        $this->assertTrue(is_string($phpVersion) && !empty($phpVersion));
    }

    public function testMagickConstants()
    {
        $t = new \Test\Constants();

        $this->assertTrue($t->testClassMagicConstant() == 'Constants');
        $this->assertTrue($t->testMethodMagicConstant() == 'Constants:testMethodMagicConstant');
        $this->assertTrue($t->testFunctionMagicConstant() == 'testFunctionMagicConstant');
        $this->assertTrue($t->testNamespaceMagicConstant() == 'Test');
    }
}
