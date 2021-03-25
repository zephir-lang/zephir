<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\ConstantsInterface;
use Stub\TestInterface;

final class ConstantsInterfaceTest extends TestCase
{
    private ConstantsInterface $test;

    protected function setUp(): void
    {
        $this->test = new ConstantsInterface();
    }

    public function testConstants(): void
    {
        $this->assertSame(TestInterface::C1, null);
        $this->assertSame(TestInterface::C2, false);
        $this->assertSame(TestInterface::C3, true);
        $this->assertSame(TestInterface::C4, 10);
        $this->assertSame(TestInterface::C5, 10.25);
        $this->assertSame(TestInterface::C6, 'test');
    }

    public function testClassOfInterface(): void
    {
        $this->assertIsObject($this->test);
        $this->assertSame($this->test->testReadInterfaceConstant1(), null);
        $this->assertSame($this->test->testReadInterfaceConstant2(), false);
        $this->assertSame($this->test->testReadInterfaceConstant3(), true);
        $this->assertSame($this->test->testReadInterfaceConstant4(), 10);
        $this->assertSame($this->test->testReadInterfaceConstant5(), 10.25);
        $this->assertSame($this->test->testReadInterfaceConstant6(), 'test');
    }

    public function testStaticConstantsInheritanceFromInterface(): void
    {
        $this->assertSame(ConstantsInterface::C1, null);
        $this->assertSame(ConstantsInterface::C2, false);
        $this->assertSame(ConstantsInterface::C3, true);
        $this->assertSame(ConstantsInterface::C4, 10);
        $this->assertSame(ConstantsInterface::C5, 10.25);
        $this->assertSame(ConstantsInterface::C6, 'test');
    }

    public function testInstanceConstantsInheritanceFromInterface(): void
    {
        $this->assertIsObject($this->test);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant1(), null);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant2(), false);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant3(), true);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant4(), 10);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant5(), 10.25);
        $this->assertSame($this->test->testReadInheritanceFromInterfaceConstant6(), 'test');
    }
}
