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
use Stub\Cast;

class CastTest extends TestCase
{
    /**
     * @var Cast
     */
    private $test;

    protected function setUp(): void
    {
        $this->test = new Cast();
    }

    protected function tearDown(): void
    {
        $this->test = null;
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1988
     */
    public function testCharCast()
    {
        /*
         * Value
         */

        $this->assertSame(97, $this->test->testCharCastFromChar());

        /*
         * Variable types
         */

        $this->assertSame(65, $this->test->testCharCastFromVariableChar());
    }

    public function testStringCast()
    {
        /*
         * Value
         */

        // https://github.com/phalcon/zephir/issues/1988
        $this->assertSame('z', $this->test->testStringCastChar());

        /*
         * Variable types
         */

        // https://github.com/phalcon/zephir/issues/1988
        $this->assertSame('X', $this->test->testStringCastVariableChar());
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
        $this->assertSame(65, $this->test->testIntCastFromChar());

        /*
         * Variable types
         */

        $this->assertSame(5, $this->test->testIntCastFromVariableFloat());
        $this->assertSame(1, $this->test->testIntCastFromVariableBooleanTrue());
        $this->assertSame(0, $this->test->testIntCastFromVariableBooleanFalse());
        $this->assertSame(0, $this->test->testIntCastFromVariableNull());

        // https://github.com/phalcon/zephir/issues/1988
        $this->assertSame(97, $this->test->testIntCastFromVariableChar());

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

    public function testLongCast()
    {
        /*
         * Value
         */

        // https://github.com/phalcon/zephir/issues/1988
        $this->assertSame(97, $this->test->testLongCastFromChar());

        /*
         * Variable types
         */

        // https://github.com/phalcon/zephir/issues/1988
        $this->assertSame(65, $this->test->testLongCastFromVariableChar());
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

    public function testDoubleCast()
    {
        /*
         * Value
         */

        // https://github.com/phalcon/zephir/issues/1988
        $this->assertSame(97.0, $this->test->testDoubleCastFromVChar());

        /*
         * Variable types
         */

        // https://github.com/phalcon/zephir/issues/1988
        $this->assertSame(65.0, $this->test->testDoubleCastFromVariableChar());
    }

    public function testBooleanCast()
    {
        /*
         * Value
         */

        $this->assertTrue($this->test->testBooleanCastFromIntTrue1());
        $this->assertTrue($this->test->testBooleanCastFromIntTrue2());
        $this->assertFalse($this->test->testBooleanCastFromIntFalse());

        // https://github.com/phalcon/zephir/issues/1988
        $this->assertTrue($this->test->testBooleanCastFromChar());

        /*
         * Variable types
         */

        $this->assertTrue($this->test->testBooleanCastFromObject());
        $this->assertFalse($this->test->testBooleanCastFromEmptyArray());
        $this->assertTrue($this->test->testBooleanCastFromArray());
        $this->assertFalse($this->test->testBooleanCastFromNull());

        // https://github.com/phalcon/zephir/issues/1988
        $this->assertTrue($this->test->testBooleanCastFromVariableChar());
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
