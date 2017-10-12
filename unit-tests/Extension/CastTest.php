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
    public function testIntCast()
    {
        $t = new Cast();

        /**
         * Value
         */

        $this->assertSame(5, $t->testIntCastFromFloat());
        $this->assertSame(1, $t->testIntCastFromBooleanTrue());
        $this->assertSame(0, $t->testIntCastFromBooleanFalse());
        $this->assertSame(0, $t->testIntCastFromNull());
        $this->assertSame(0, $t->testIntCastFromStringValue());
        $this->assertSame(0, $t->testIntCastFromEmptyArray());
        $this->assertSame(1, $t->testIntCastFromArray());
        $this->assertSame(1, $t->testIntCastFromStdClass());


        /**
         * Variable types
         */

        $this->assertSame(5, $t->testIntCastFromVariableFloat());
        $this->assertSame(1, $t->testIntCastFromVariableBooleanTrue());
        $this->assertSame(0, $t->testIntCastFromVariableBooleanFalse());
        $this->assertSame(0, $t->testIntCastFromVariableNull());

        $this->assertSame(0, $t->testIntCastFromVariableString());
        $this->assertSame((int) "test", $t->testIntCastFromParameterString("test"));
        $this->assertSame((int) "1", $t->testIntCastFromParameterString("1"));
        $this->assertSame((int) "12345", $t->testIntCastFromParameterString("12345"));
        $this->assertSame((int) "-1", $t->testIntCastFromParameterString("-1"));
        $this->assertSame((int) "+5", $t->testIntCastFromParameterString("+5"));

        $this->assertSame(0, $t->testIntCastFromVariableEmptyArray());
        $this->assertSame(1, $t->testIntCastFromVariableArray());
        $this->assertSame(1, $t->testIntCastFromVariableStdClass());
    }

    public function testFloatCast()
    {
        $t = new Cast();

        $this->assertSame($t->testFloatCastFromFloat(), 5.0);
        $this->assertSame($t->testFloatCastFromBooleanTrue(), 1.0);
        $this->assertSame($t->testFloatCastFromBooleanFalse(), 0.0);
        $this->assertSame($t->testFloatCastFromNull(), 0.0);
        $this->assertSame($t->testFloatCastFromEmptyArray(), 0.0);
        $this->assertSame($t->testFloatCastFromArray(), 1.0);
        $this->assertSame($t->testFloatCastFromStdClass(), 1.0);

        $this->assertSame($t->testFloatCastFromVariableFloat(), 5.0);
        $this->assertSame($t->testFloatCastFromVariableBooleanTrue(), 1.0);
        $this->assertSame($t->testFloatCastFromVariableBooleanFalse(), 0.0);
        $this->assertSame($t->testFloatCastFromVariableNull(), 0.0);
        $this->assertSame($t->testFloatCastFromVariableEmptyArray(), 0.0);
        $this->assertSame($t->testFloatCastFromVariableArray(), 1.0);
        $this->assertSame($t->testFloatCastFromVariableStdClass(), 1.0);
    }

    public function testBooleanCast()
    {
        $t = new Cast();

        $this->assertTrue($t->testBooleanCastFromIntTrue1());
        $this->assertTrue($t->testBooleanCastFromIntTrue2());
        $this->assertFalse($t->testBooleanCastFromIntFalse());
        $this->assertTrue($t->testBooleanCastFromObject());
        $this->assertFalse($t->testBooleanCastFromEmptyArray());
        $this->assertTrue($t->testBooleanCastFromArray());
        $this->assertFalse($t->testBooleanCastFromNull());
    }

    public function testObjectCast()
    {
        $t = new Cast();

        $this->assertEquals((object) 5, $t->testObjectCastFromInt());
        $this->assertEquals((object) 5.0, $t->testObjectCastFromFloat());
        $this->assertEquals((object) false, $t->testObjectCastFromFalse());
        $this->assertEquals((object) true, $t->testObjectCastFromTrue());
        $this->assertEquals((object) null, $t->testObjectCastFromNull());
        $this->assertEquals((object) array(), $t->testObjectCastFromEmptyArray());
        $this->assertEquals((object) array(1, 2, 3, 4), $t->testObjectCastFromArray());
        $this->assertEquals((object) "", $t->testObjectCastFromEmptyString());
        $this->assertEquals((object) "test string", $t->testObjectCastFromString());
    }
}
