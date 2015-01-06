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


use Test\Cast;

class CastTest extends \PHPUnit_Framework_TestCase
{
    public function testIntCast()
    {
        $t = new Cast();

        $this->assertTrue($t->testIntCastFromFloat() === 5);
        $this->assertTrue($t->testIntCastFromBooleanTrue() === 1);
        $this->assertTrue($t->testIntCastFromBooleanFalse() === 0);
        $this->assertTrue($t->testIntCastFromNull() === 0);
        $this->assertTrue($t->testIntCastFromEmptyArray() === 0);
        $this->assertTrue($t->testIntCastFromArray() === 1);
        $this->assertTrue($t->testIntCastFromStdClass() === 1);

        $this->assertTrue($t->testIntCastFromVariableFloat() === 5);
        $this->assertTrue($t->testIntCastFromVariableBooleanTrue() === 1);
        $this->assertTrue($t->testIntCastFromVariableBooleanFalse() === 0);
        $this->assertTrue($t->testIntCastFromVariableNull() === 0);
        $this->assertTrue($t->testIntCastFromVariableEmptyArray() === 0);
        $this->assertTrue($t->testIntCastFromVariableArray() === 1);
        $this->assertTrue($t->testIntCastFromVariableStdClass() === 1);
    }

    public function testFloatCast()
    {
        $t = new Cast();

        $this->assertTrue($t->testFloatCastFromFloat() === 5.0);
        $this->assertTrue($t->testFloatCastFromBooleanTrue() === 1.0);
        $this->assertTrue($t->testFloatCastFromBooleanFalse() === 0.0);
        $this->assertTrue($t->testFloatCastFromNull() === 0.0);
        $this->assertTrue($t->testFloatCastFromEmptyArray() === 0.0);
        $this->assertTrue($t->testFloatCastFromArray() === 1.0);
        $this->assertTrue($t->testFloatCastFromStdClass() === 1.0);

        $this->assertTrue($t->testFloatCastFromVariableFloat() === 5.0);
        $this->assertTrue($t->testFloatCastFromVariableBooleanTrue() === 1.0);
        $this->assertTrue($t->testFloatCastFromVariableBooleanFalse() === 0.0);
        $this->assertTrue($t->testFloatCastFromVariableNull() === 0.0);
        $this->assertTrue($t->testFloatCastFromVariableEmptyArray() === 0.0);
        $this->assertTrue($t->testFloatCastFromVariableArray() === 1.0);
        $this->assertTrue($t->testFloatCastFromVariableStdClass() === 1.0);
    }

    public function testBooleanCast()
    {
        $t = new Cast();

        $this->assertTrue($t->testBooleanCastFromIntTrue1() === true);
        $this->assertTrue($t->testBooleanCastFromIntTrue2() === true);
        $this->assertTrue($t->testBooleanCastFromIntFalse() === false);
        $this->assertTrue($t->testBooleanCastFromObject() === true);
        $this->assertTrue($t->testBooleanCastFromEmptyArray() === false);
        $this->assertTrue($t->testBooleanCastFromArray() === true);
        $this->assertTrue($t->testBooleanCastFromNull() === false);
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
