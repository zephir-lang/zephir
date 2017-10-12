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

class ConstantsInterfaceTest extends \PHPUnit_Framework_TestCase
{
    public function testConstants()
    {
        $this->assertSame(\Test\TestInterface::C1, null);
        $this->assertSame(\Test\TestInterface::C2, false);
        $this->assertSame(\Test\TestInterface::C3, true);
        $this->assertSame(\Test\TestInterface::C4, 10);
        $this->assertSame(\Test\TestInterface::C5, 10.25);
        $this->assertSame(\Test\TestInterface::C6, 'test');
    }

    public function testClassOfInterface()
    {
        $t = new \Test\ConstantsInterface();

        $this->assertTrue(is_object($t));
        $this->assertSame($t->testReadInterfaceConstant1(), null);
        $this->assertSame($t->testReadInterfaceConstant2(), false);
        $this->assertSame($t->testReadInterfaceConstant3(), true);
        $this->assertSame($t->testReadInterfaceConstant4(), 10);
        $this->assertSame($t->testReadInterfaceConstant5(), 10.25);
        $this->assertSame($t->testReadInterfaceConstant6(), 'test');
    }

    public function testStaticConstantsInheritanceFromInterface()
    {
        $this->assertSame(\Test\ConstantsInterface::C1, null);
        $this->assertSame(\Test\ConstantsInterface::C2, false);
        $this->assertSame(\Test\ConstantsInterface::C3, true);
        $this->assertSame(\Test\ConstantsInterface::C4, 10);
        $this->assertSame(\Test\ConstantsInterface::C5, 10.25);
        $this->assertSame(\Test\ConstantsInterface::C6, 'test');
    }

    public function testInstanceConstantsInheritanceFromInterface()
    {
        $t = new \Test\ConstantsInterface();

        $this->assertTrue(is_object($t));
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant1(), null);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant2(), false);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant3(), true);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant4(), 10);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant5(), 10.25);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant6(), 'test');
    }
}
