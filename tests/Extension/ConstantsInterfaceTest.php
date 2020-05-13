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
    /** @var \Stub\ConstantsInterface */
    private $test;

    public function setUp()
    {
        $this->test = new \Stub\ConstantsInterface();
    }

    public function testConstants()
    {
        $this->assertSame(\Stub\TestInterface::C1, null);
        $this->assertSame(\Stub\TestInterface::C2, false);
        $this->assertSame(\Stub\TestInterface::C3, true);
        $this->assertSame(\Stub\TestInterface::C4, 10);
        $this->assertSame(\Stub\TestInterface::C5, 10.25);
        $this->assertSame(\Stub\TestInterface::C6, 'test');
    }

    public function testClassOfInterface()
    {
        $this->assertInternalType('object', $this->test);
        $this->assertSame($this->test->testReadInterfaceConstant1(), null);
        $this->assertSame($this->test->testReadInterfaceConstant2(), false);
        $this->assertSame($this->test->testReadInterfaceConstant3(), true);
        $this->assertSame($this->test->testReadInterfaceConstant4(), 10);
        $this->assertSame($this->test->testReadInterfaceConstant5(), 10.25);
        $this->assertSame($this->test->testReadInterfaceConstant6(), 'test');
    }

    public function testStaticConstantsInheritanceFromInterface()
    {
        $this->assertSame(\Stub\ConstantsInterface::C1, null);
        $this->assertSame(\Stub\ConstantsInterface::C2, false);
        $this->assertSame(\Stub\ConstantsInterface::C3, true);
        $this->assertSame(\Stub\ConstantsInterface::C4, 10);
        $this->assertSame(\Stub\ConstantsInterface::C5, 10.25);
        $this->assertSame(\Stub\ConstantsInterface::C6, 'test');
    }

    public function testInstanceConstantsInheritanceFromInterface()
    {
        $this->assertInternalType('object', $this->test);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant1(), null);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant2(), false);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant3(), true);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant4(), 10);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant5(), 10.25);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant6(), 'test');
    }
}
