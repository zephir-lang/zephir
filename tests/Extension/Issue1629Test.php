<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Issue1629;

/**
 * A string offset read boxes as PHP's 1-char string; a declared char/uchar
 * variable boxes as its integer byte value.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/1629
 */
final class Issue1629Test extends TestCase
{
    private Issue1629 $test;

    protected function setUp(): void
    {
        $this->test = new Issue1629();
    }

    public function testStringOffsetIntoStringTarget(): void
    {
        $this->assertSame('e', $this->test->stringTarget('zephir', 1));
    }

    public function testStringOffsetIntoDynamicTarget(): void
    {
        // PHP's own "zephir"[1] is the string 'e', not the byte 101.
        $this->assertSame('e', $this->test->varTarget('zephir', 1));
    }

    public function testStringOffsetAppendedToArray(): void
    {
        $this->assertSame('zephir', $this->test->rebuild('zephir'));
    }

    public function testDeclaredCharKeepsItsByteValue(): void
    {
        $this->assertSame([101], $this->test->charTarget('zephir', 1));
    }

    public function testNativeTargetReadsTheRawByte(): void
    {
        $this->assertSame(101, $this->test->intTarget('zephir', 1));
    }

    public function testStringOffsetIntoArrayOffset(): void
    {
        $this->assertSame([3 => 'e'], $this->test->arrayIndexTarget('zephir', 1));
    }

    public function testStringOffsetIntoPropertyArray(): void
    {
        $this->assertSame(['e'], $this->test->propertyAppendTarget('zephir', 1));
        $this->assertSame(['k' => 'e'], $this->test->propertyIndexTarget('zephir', 1));
    }

    public function testStringOffsetIntoStaticPropertyArray(): void
    {
        $this->assertSame([0 => 'e'], Issue1629::staticIndexTarget('zephir', 1));
        $this->assertSame(['e'], Issue1629::staticAppendTarget('zephir', 1));
    }

    public function testDeclaredCharIntoStringTarget(): void
    {
        $this->assertSame('e', $this->test->charToStringTarget('zephir', 1));
    }

    public function testIntIntoStringTarget(): void
    {
        $this->assertSame('42', $this->test->intToStringTarget());
    }

    public function testStringCast(): void
    {
        $this->assertSame('e', $this->test->castStringOfChar('zephir', 1));
        $this->assertSame('e', $this->test->castStringOfOffset('zephir', 1));
    }

    public function testCharCasts(): void
    {
        $this->assertSame(97, $this->test->castCharOfVar(97));
        $this->assertSame(200, $this->test->castUcharOfVar(200));
    }

    public function testUcharParameter(): void
    {
        $this->assertSame(97, $this->test->ucharParam('a'));
        // An unsigned char keeps byte values above 127 (a signed char would
        // sign-extend 200 to -56).
        $this->assertSame(200, $this->test->ucharParam(200));
    }

    public function testCharInKeyedArrayLiteral(): void
    {
        $this->assertSame([1 => 101, 'k' => 101], $this->test->keyedCharArray('zephir', 1));
    }

    public function testCharIntoDoubleAndBoolTargets(): void
    {
        $this->assertSame(101.0, $this->test->doubleFromChar('zephir', 1));
        $this->assertTrue($this->test->boolFromChar('zephir', 1));
    }

    public function testCharCastIntoDynamicVariable(): void
    {
        $this->assertSame(97, $this->test->dynamicFromCharCast());
    }

    public function testBareCharLiteral(): void
    {
        $this->assertSame(97, $this->test->returnCharLiteral());
        $this->assertSame([97], $this->test->appendCharLiteral());
    }
}
