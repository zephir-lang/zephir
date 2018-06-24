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

use Test\Cast;

class CastTest extends \PHPUnit_Framework_TestCase
{
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
        /**
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


        /**
         * Variable types
         */

        $this->assertSame(5, $this->test->testIntCastFromVariableFloat());
        $this->assertSame(1, $this->test->testIntCastFromVariableBooleanTrue());
        $this->assertSame(0, $this->test->testIntCastFromVariableBooleanFalse());
        $this->assertSame(0, $this->test->testIntCastFromVariableNull());

        $this->assertSame(0, $this->test->testIntCastFromVariableString());
        $this->assertSame((int) "test", $this->test->testIntCastFromParameterString("test"));
        $this->assertSame((int) "1", $this->test->testIntCastFromParameterString("1"));
        $this->assertSame((int) "12345", $this->test->testIntCastFromParameterString("12345"));
        $this->assertSame((int) "-1", $this->test->testIntCastFromParameterString("-1"));
        $this->assertSame((int) "+5", $this->test->testIntCastFromParameterString("+5"));

        $this->assertSame(0, $this->test->testIntCastFromVariableEmptyArray());
        $this->assertSame(1, $this->test->testIntCastFromVariableArray());
        $this->assertSame(1, $this->test->testIntCastFromVariableStdClass());
    }

    public function testFloatCast()
    {
        $this->assertSame($this->test->testFloatCastFromFloat(), 5.0);
        $this->assertSame($this->test->testFloatCastFromBooleanTrue(), 1.0);
        $this->assertSame($this->test->testFloatCastFromBooleanFalse(), 0.0);
        $this->assertSame($this->test->testFloatCastFromNull(), 0.0);
        $this->assertSame($this->test->testFloatCastFromEmptyArray(), 0.0);
        $this->assertSame($this->test->testFloatCastFromArray(), 1.0);
        $this->assertSame($this->test->testFloatCastFromStdClass(), 1.0);

        $this->assertSame($this->test->testFloatCastFromVariableFloat(), 5.0);
        $this->assertSame($this->test->testFloatCastFromVariableBooleanTrue(), 1.0);
        $this->assertSame($this->test->testFloatCastFromVariableBooleanFalse(), 0.0);
        $this->assertSame($this->test->testFloatCastFromVariableNull(), 0.0);
        $this->assertSame($this->test->testFloatCastFromVariableEmptyArray(), 0.0);
        $this->assertSame($this->test->testFloatCastFromVariableArray(), 1.0);
        $this->assertSame($this->test->testFloatCastFromVariableStdClass(), 1.0);
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
        $this->assertEquals((object) array(), $this->test->testObjectCastFromEmptyArray());
        $this->assertEquals((object) array(1, 2, 3, 4), $this->test->testObjectCastFromArray());
        $this->assertEquals((object) "", $this->test->testObjectCastFromEmptyString());
        $this->assertEquals((object) "test string", $this->test->testObjectCastFromString());
    }

    /**
     * Test cases for Issue 1524
     * Casting Resource to Integer must always return int value of Resource Id
     *
     * @author Alexander Andriiako <AlexNDR@phalconphp.com>
     * @return void
     */
    public function testResourceCast()
    {
        $file = fopen(__DIR__ . '/php/exists.php', 'r');

        $this->assertEquals((int) STDIN, $this->test->testCastStdinToInteger());
        $this->assertEquals((int) STDOUT, $this->test->testCastStdoutToInteger());
        $this->assertEquals((int) $file, $this->test->testCastFileResourceToInteger($file));

        fclose($file);
    }
}
