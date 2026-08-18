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
use Stub\Cast;

final class CastTest extends TestCase
{
    private ?Cast $test = null;

    protected function setUp(): void
    {
        $this->test = new Cast();
    }

    protected function tearDown(): void
    {
        $this->test = null;
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testCharCast(): void
    {
        /**
         * Value
         */
        $this->assertSame(97, $this->test->testCharCastFromChar());

        /**
         * Variable types
         */
        $this->assertSame(65, $this->test->testCharCastFromVariableChar());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testStringCast(): void
    {
        /**
         * Value
         */
        $this->assertSame('z', $this->test->testStringCastChar());

        /**
         * Variable types
         */
        $this->assertSame('X', $this->test->testStringCastVariableChar());

        /**
         * Null
         */
        $this->assertSame('', $this->test->testStringCastFromNull());
    }

    public function testIntCast(): void
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
        $this->assertSame(65, $this->test->testIntCastFromChar());

        /**
         * Variable types
         */
        $this->assertSame(5, $this->test->testIntCastFromVariableFloat());
        $this->assertSame(1, $this->test->testIntCastFromVariableBooleanTrue());
        $this->assertSame(0, $this->test->testIntCastFromVariableBooleanFalse());
        $this->assertSame(0, $this->test->testIntCastFromVariableNull());

        /**
         * @issue https://github.com/zephir-lang/zephir/issues/1988
         */
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

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testLongCast(): void
    {
        /**
         * Value
         */
        $this->assertSame(97, $this->test->testLongCastFromChar());

        /**
         * Variable types
         */
        $this->assertSame(65, $this->test->testLongCastFromVariableChar());
    }

    public function testFloatCast(): void
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

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1988
     */
    public function testDoubleCast(): void
    {
        /**
         * Value
         */
        $this->assertSame(97.0, $this->test->testDoubleCastFromVChar());

        /**
         * Variable types
         */
        $this->assertSame(65.0, $this->test->testDoubleCastFromVariableChar());
    }

    public function testBooleanCast(): void
    {
        /**
         * Value
         */
        $this->assertTrue($this->test->testBooleanCastFromIntTrue1());
        $this->assertTrue($this->test->testBooleanCastFromIntTrue2());
        $this->assertFalse($this->test->testBooleanCastFromIntFalse());

        /**
         * @issue https://github.com/zephir-lang/zephir/issues/1988
         */
        $this->assertTrue($this->test->testBooleanCastFromChar());

        /**
         * Variable types
         */
        $this->assertTrue($this->test->testBooleanCastFromObject());
        $this->assertFalse($this->test->testBooleanCastFromEmptyArray());
        $this->assertTrue($this->test->testBooleanCastFromArray());
        $this->assertFalse($this->test->testBooleanCastFromNull());

        /**
         * @issue https://github.com/zephir-lang/zephir/issues/1988
         */
        $this->assertTrue($this->test->testBooleanCastFromVariableChar());
    }

    public function testObjectCast(): void
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

    public function testArrayCast(): void
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

    public function testIssue828(): void
    {
        $return = $this->test->testIssue828();

        $this->assertSame(['1.0 200 OK', 'OK', 1.0, 1, 0.0, 0], $return);
        $this->assertSame('1.0 200 OK', $return[0]);
        $this->assertSame('OK', $return[1]);
        $this->assertSame(1.0, $return[2]);
        $this->assertSame(1, $return[3]);
        $this->assertSame(0.0, $return[4]);
        $this->assertSame(0, $return[5]);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1877
     */
    public function testObjectCastThenReassign(): void
    {
        $this->assertSame('reassigned', $this->test->testObjectCastThenReassign());
    }

    /**
     * Casts to `int` from every source form.
     *
     * Expectations are computed with native PHP casts rather than written out,
     * so the suite pins parity with the engine instead of pinning whatever the
     * compiler currently emits. The two hardcoded 122s are the `char` sources:
     * Zephir's `char` is a byte and PHP has no equivalent.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToInt(): void
    {
        $i = 5;
        $l = 6;
        $d = 5.5;
        $b = true;
        $s = '12abc';
        $arr = [1, 2];

        $this->assertSame(
            [
                (int) 5, (int) 5.5, (int) true, (int) false, (int) null, (int) '12abc',
                122, (int) [1, 2], (int) [],
                (int) $i, (int) $l, (int) $d, (int) $b, (int) $s, 122, (int) $arr,
                (int) ($i + 1), (int) '7abc', 3,
            ],
            $this->test->issue1841ToInt('7abc')
        );
    }

    /**
     * `uint`/`ulong` had no branch at all, so every source failed to compile.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToUnsigned(): void
    {
        $this->assertSame(
            [
                5, 5, 1, 0, 12, 122, 5, 5, 12, 122, 7,
                5, 5, 1, 0, 12, 5, 122, 7,
            ],
            $this->test->issue1841ToUnsigned('7abc')
        );
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToLong(): void
    {
        $i = 5;
        $l = 6;
        $d = 5.5;
        $b = true;
        $s = '12abc';
        $arr = [1, 2];

        $this->assertSame(
            [
                (int) 5, (int) 5.5, (int) true, (int) null, (int) '12abc', 122, (int) [1, 2],
                (int) $i, (int) $l, (int) $d, (int) $b, (int) $s, 122, (int) $arr,
                (int) ($i + 1), (int) '7abc', 3,
            ],
            $this->test->issue1841ToLong('7abc')
        );
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToFloat(): void
    {
        $i = 5;
        $l = 6;
        $d = 5.5;
        $b = true;
        $s = '12abc';
        $arr = [1, 2];

        $this->assertSame(
            [
                (float) 5, (float) 5.5, (float) true, (float) false, (float) null, (float) '5.5abc',
                122.0, (float) [1, 2], (float) [],
                (float) $i, (float) $l, (float) $d, (float) $b, (float) $s, 122.0, (float) $arr,
                (float) ($i + 1), (float) '7abc', 3.0,
            ],
            $this->test->issue1841ToFloat('7abc')
        );
    }

    /**
     * A C cast truncates, so `(bool) 0.4` must not lower to one — PHP treats
     * every non-zero double as true.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToBool(): void
    {
        $i = 5;
        $l = 6;
        $d = 5.5;
        $b = true;
        $s = '12abc';
        $arr = [1, 2];

        $actual = $this->test->issue1841ToBool('7abc');

        $this->assertSame(
            [
                (bool) 1, (bool) 0, (bool) 0.4, (bool) 0.0, (bool) true, (bool) false,
                (bool) null, (bool) '0', (bool) '', (bool) 'abc', true,
                (bool) [1, 2], (bool) [],
                (bool) $i, (bool) $l, (bool) $d, (bool) $b, (bool) $s, (bool) '0', true,
                (bool) $arr, (bool) '7abc', (bool) 3,
            ],
            $actual
        );

        // The truncation trap, called out on its own so a regression is obvious.
        $this->assertTrue($actual[2]);
    }

    /**
     * `char`/`uchar` is a byte, so a cast to it follows `(int)` and boxes as the
     * integer byte value. `(uchar) 200` stays 200 where a signed `char` would
     * not, which is the whole point of having both.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1629
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToChar(): void
    {
        $this->assertSame(
            [
                65, 122, 122, 1, 0, 0, 65, 5, 122, 65, 7, 65,
                65, 122, 200, 0, 5, 122, 7,
            ],
            $this->test->issue1841ToChar('7abc')
        );
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToString(): void
    {
        $i = 5;
        $l = 6;
        $d = 5.5;
        $b = true;
        $s = '12abc';

        $this->assertSame(
            [
                (string) 5, (string) 5.0, (string) 5.5, (string) true, (string) false, (string) null,
                (string) 'abc', 'z',
                (string) $i, (string) $l, (string) $d, (string) $b, (string) $s, 'z',
                (string) ($i + 1), '7abc', (string) 3,
            ],
            $this->test->issue1841ToString('7abc')
        );
    }

    /**
     * PHP renders an array as "Array" and raises a warning while doing it.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToStringFromArray(): void
    {
        $this->assertSame(['Array', 'Array'], @$this->test->issue1841ToStringFromArray());
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToArray(): void
    {
        $i = 5;
        $s = '12abc';
        $arr = [1, 2];

        $this->assertSame(
            [
                (array) 5, (array) 5.5, (array) true, (array) null, (array) 'abc', (array) 122,
                (array) [1, 2], (array) [],
                (array) $i, (array) $s, (array) 122, (array) $arr, (array) '7abc',
            ],
            $this->test->issue1841ToArray('7abc')
        );
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToObject(): void
    {
        $this->assertEquals(
            [
                (object) 5, (object) null, (object) [1, 2], (object) 'x', (object) 122,
                (object) 5, (object) 'abc', (object) 122, (object) '7abc',
            ],
            $this->test->issue1841ToObject('7abc')
        );
    }

    /**
     * `var` accepts anything, so `(var)` is a no-op that has to preserve both
     * the value and its type.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841ToVar(): void
    {
        $this->assertSame(
            [5, 5.5, true, null, 'abc', 122, [1, 2], 5, 'abc', 122, [1, 2], '7abc'],
            $this->test->issue1841ToVar('7abc')
        );
    }

    /**
     * `(array)`/`(object)` lower to kernel conversions that run in place, so
     * casting a variable used to overwrite that variable as a side effect.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841CastKeepsOperandIntact(): void
    {
        [$value, $asArray, $asObject] = $this->test->issue1841CastKeepsOperandIntact(5);

        $this->assertSame(5, $value);
        $this->assertSame([5], $asArray);
        $this->assertEquals((object) 5, $asObject);
    }

    /**
     * The reported symptoms: a cast (or a parenthesized expression) used as a
     * variable declaration's default value failed with "Unknown type: cast" /
     * "Unknown type: list".
     *
     * @see https://github.com/zephir-lang/zephir/issues/1841
     */
    public function testIssue1841DeclarationInitializers(): void
    {
        $this->assertSame(
            [4, 4.0, 21, '10', 7, 10, 10],
            $this->test->issue1841DeclarationInitializers(7, 3)
        );
    }
}
