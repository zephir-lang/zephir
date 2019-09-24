<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Test\Cast;

class CastTest extends TestCase
{
    /**
     * @var Cast
     */
    private $test;

    public function setUp()
    {
        $this->test = new Cast();
    }

    public function tearDown()
    {
        $this->test = null;
    }

    public function testIntCast()
    {
        /*
         * Value
         */
        $this->assertSame(5, $this->test->testIntCastFromFloat());
        $this->assertSame(1, $this->test->testIntCastFromBooleanTrue());
        $this->assertSame(0, $this->test->testIntCastFromBooleanFalse());
        $this->assertSame(0, $this->test->testIntCastFromNull());
        $this->assertSame(0, $this->test->testIntCastFromStringValue());
        $this->assertSame(0, $this->test->testIntCastFromEmptyArray());
        $this->assertSame(1, $this->test->testIntCastFromArray());
        $this->assertSame(1, $this->test->testIntCastFromStdClass());

        /*
         * Variable types
         */
        $this->assertSame(5, $this->test->testIntCastFromVariableFloat());
        $this->assertSame(1, $this->test->testIntCastFromVariableBooleanTrue());
        $this->assertSame(0, $this->test->testIntCastFromVariableBooleanFalse());
        $this->assertSame(0, $this->test->testIntCastFromVariableNull());

        $this->assertSame(0, $this->test->testIntCastFromVariableString());
        $this->assertSame((int) 'test', $this->test->testIntCastFromParameterString('test'));
        $this->assertSame((int) '1', $this->test->testIntCastFromParameterString('1'));
        $this->assertSame((int) '12345', $this->test->testIntCastFromParameterString('12345'));
        $this->assertSame((int) '-1', $this->test->testIntCastFromParameterString('-1'));
        $this->assertSame((int) '+5', $this->test->testIntCastFromParameterString('+5'));

        $this->assertSame(0, $this->test->testIntCastFromVariableEmptyArray());
        $this->assertSame(1, $this->test->testIntCastFromVariableArray());
        $this->assertSame(1, $this->test->testIntCastFromVariableStdClass());
    }

    public function testFloatCast()
    {
        $this->assertSame(5.0, $this->test->testFloatCastFromFloat());
        $this->assertSame(1.0, $this->test->testFloatCastFromBooleanTrue());
        $this->assertSame(0.0, $this->test->testFloatCastFromBooleanFalse());
        $this->assertSame(0.0, $this->test->testFloatCastFromNull());
        $this->assertSame(0.0, $this->test->testFloatCastFromEmptyArray());
        $this->assertSame(1.0, $this->test->testFloatCastFromArray());
        $this->assertSame(1.0, $this->test->testFloatCastFromStdClass());

        $this->assertSame(5.0, $this->test->testFloatCastFromVariableFloat());
        $this->assertSame(1.0, $this->test->testFloatCastFromVariableBooleanTrue());
        $this->assertSame(0.0, $this->test->testFloatCastFromVariableBooleanFalse());
        $this->assertSame(0.0, $this->test->testFloatCastFromVariableNull());
        $this->assertSame(0.0, $this->test->testFloatCastFromVariableEmptyArray());
        $this->assertSame(1.0, $this->test->testFloatCastFromVariableArray());
        $this->assertSame(1.0, $this->test->testFloatCastFromVariableStdClass());
    }

    public function testBooleanCast()
    {
        $this->assertTrue($this->test->testBooleanCastFromIntTrue1());
        $this->assertTrue($this->test->testBooleanCastFromIntTrue2());
        $this->assertFalse($this->test->testBooleanCastFromIntFalse());
        $this->assertTrue($this->test->testBooleanCastFromObject());
        $this->assertFalse($this->test->testBooleanCastFromEmptyArray());
        $this->assertTrue($this->test->testBooleanCastFromArray());
        $this->assertFalse($this->test->testBooleanCastFromNull());
    }

    public function testObjectCast()
    {
        $this->assertEquals((object) 5, $this->test->testObjectCastFromInt());
        $this->assertEquals((object) 5.0, $this->test->testObjectCastFromFloat());
        $this->assertEquals((object) false, $this->test->testObjectCastFromFalse());
        $this->assertEquals((object) true, $this->test->testObjectCastFromTrue());
        $this->assertEquals((object) null, $this->test->testObjectCastFromNull());
        $this->assertEquals((object) [], $this->test->testObjectCastFromEmptyArray());
        $this->assertEquals((object) [1, 2, 3, 4], $this->test->testObjectCastFromArray());
        $this->assertEquals((object) '', $this->test->testObjectCastFromEmptyString());
        $this->assertEquals((object) 'test string', $this->test->testObjectCastFromString());
    }

    public function testArrayCast()
    {
        $this->assertEquals((array) [1, 2, 3], $this->test->testArrayCastFromVariableArray());
        $this->assertEquals((array) true, $this->test->testArrayCastFromVariableTrue());
        $this->assertEquals((array) false, $this->test->testArrayCastFromVariableFalse());
        $this->assertEquals((array) null, $this->test->testArrayCastFromVariableNull());
        $this->assertEquals((array) 1, $this->test->testArrayCastFromVariableInteger());
        $this->assertEquals((array) 1.1, $this->test->testArrayCastFromVariableFloat());
        $this->assertEquals((array) 'aaa', $this->test->testArrayCastFromVariableString());
        $this->assertEquals((array) ['p1' => 'v1', 'p2' => 'v2'], $this->test->testArrayCastFromVariableStdClass());
    }
}
