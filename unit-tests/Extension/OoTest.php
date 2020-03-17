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
use Test\Oo;

class OoTest extends TestCase
{
    public function testAssertations()
    {
        $test = new \Test\Oo();

        $this->assertEquals($test->testInstance1(), new \stdClass());
        $this->assertEquals($test->testInstance2(), new \Test\Oo\OoConstruct());
        $this->assertEquals($test->testInstance3(), new \Test\Oo\OoNoConstruct());

        $obj4 = $test->testInstance4();
        $this->assertInternalType('object', $obj4);
        $this->assertSame($obj4->getA(), 'a');
        $this->assertSame($obj4->getB(), 'b');

        $obj5 = $test->testInstance5();
        $this->assertInternalType('object', $obj5);
        $this->assertSame($obj5->getA(), 'a');
        $this->assertSame($obj5->getB(), 'b');

        $obj6 = $test->testInstance6();
        $this->assertInternalType('object', $obj6);
        $this->assertSame($obj6->getA(), 1);
        $this->assertSame($obj6->getB(), 2);

        $obj7 = $test->testInstance7();
        $this->assertInternalType('object', $obj7);
        $this->assertSame($obj7->getA(), false);
        $this->assertSame($obj7->getB(), true);

        $obj8 = $test->testInstance8();
        $this->assertInternalType('object', $obj8);
        $this->assertSame($obj8->getA(), 1.2);
        $this->assertSame($obj8->getB(), 7.30);

        $obj9 = $test->testInstance9();
        $this->assertInternalType('object', $obj9);
        $this->assertInstanceOf('Test\Oo\OoDynamicA', $obj9);

        $obj10 = $test->testInstance10();
        $this->assertInternalType('object', $obj10);
        $this->assertInstanceOf('Test\Oo\OoDynamicB', $obj10);

        $obj = $test->testInstance11();
        $this->assertInternalType('object', $obj);
        $this->assertInstanceOf('Test\Oo\OoConstructParams', $obj);

        $obj12 = $test->testInstance12();
        $this->assertInternalType('object', $obj12);
        $this->assertInstanceOf('Test\Oo\OoDynamicA', $obj12);
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1673
     */
    public function shouldDinamicalyCreateInstancesInLoop()
    {
        $test = new Oo();

        $this->assertEquals(
            ['A', 'AA', 'B', 'BB'],
            $test->createInstancesInLoop()
        );
    }
}
