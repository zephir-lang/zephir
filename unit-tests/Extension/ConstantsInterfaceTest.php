<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class ConstantsInterfaceTest extends TestCase
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

        $this->assertInternalType('object', $t);
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

        $this->assertInternalType('object', $t);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant1(), null);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant2(), false);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant3(), true);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant4(), 10);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant5(), 10.25);
        $this->assertSame($t->testReadInheritanceFromInterfaceConstant6(), 'test');
    }
}
