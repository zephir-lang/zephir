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
use Stub\Oo;

class OoTest extends TestCase
{
    public function testAssertations(): void
    {
        $test = new \Stub\Oo();

        $this->assertEquals($test->testInstance1(), new \stdClass());
        $this->assertEquals($test->testInstance2(), new \Stub\Oo\OoConstruct());
        $this->assertEquals($test->testInstance3(), new \Stub\Oo\OoNoConstruct());

        $obj4 = $test->testInstance4();
        $this->assertIsObject($obj4);
        $this->assertSame($obj4->getA(), 'a');
        $this->assertSame($obj4->getB(), 'b');

        $obj5 = $test->testInstance5();
        $this->assertIsObject($obj5);
        $this->assertSame($obj5->getA(), 'a');
        $this->assertSame($obj5->getB(), 'b');

        $obj6 = $test->testInstance6();
        $this->assertIsObject($obj6);
        $this->assertSame($obj6->getA(), 1);
        $this->assertSame($obj6->getB(), 2);

        $obj7 = $test->testInstance7();
        $this->assertIsObject($obj7);
        $this->assertSame($obj7->getA(), false);
        $this->assertSame($obj7->getB(), true);

        $obj8 = $test->testInstance8();
        $this->assertIsObject($obj8);
        $this->assertSame($obj8->getA(), 1.2);
        $this->assertSame($obj8->getB(), 7.30);

        $obj9 = $test->testInstance9();
        $this->assertIsObject($obj9);
        $this->assertInstanceOf(\Stub\Oo\OoDynamicA::class, $obj9);

        $obj10 = $test->testInstance10();
        $this->assertIsObject($obj10);
        $this->assertInstanceOf(\Stub\Oo\OoDynamicB::class, $obj10);

        $obj = $test->testInstance11();
        $this->assertIsObject($obj);
        $this->assertInstanceOf(\Stub\Oo\OoConstructParams::class, $obj);

        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            /*$this->markTestIncomplete(
                'Not ready to run with PHP 8: interrupted by signal 11: SIGSEGV (Invalid callback , no array or string given)'
            );*/
        }

        $obj12 = $test->testInstance12();
        $this->assertIsObject($obj12);
        $this->assertInstanceOf('Stub\Oo\OoDynamicA', $obj12);
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1673
     */
    public function testShouldDinamicalyCreateInstancesInLoop(): void
    {
        $test = new Oo();

        $this->assertSame(
            ['A', 'AA', 'B', 'BB'],
            $test->createInstancesInLoop()
        );
    }
}
