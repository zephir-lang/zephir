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
use Stub\Issue2409;

/**
 * Assigning an empty string to a static property must compile and behave
 * like any other string assignment.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2409
 */
final class Issue2409Test extends TestCase
{
    public function testAssignEmptyStringToStaticProperty(): void
    {
        Issue2409::setEmpty();
        $this->assertSame('', Issue2409::getVar());
    }

    public function testAssignNonEmptyStringToStaticProperty(): void
    {
        Issue2409::setNonEmpty();
        $this->assertSame('123', Issue2409::getVar());
    }

    public function testAssignNullToStaticProperty(): void
    {
        Issue2409::setNull();
        $this->assertNull(Issue2409::getVar());
    }

    public function testAssignIntToStaticProperty(): void
    {
        Issue2409::setInt();
        $this->assertSame(42, Issue2409::getVar());
    }

    public function testAssignNegativeIntToStaticProperty(): void
    {
        Issue2409::setNegativeInt();
        $this->assertSame(-7, Issue2409::getVar());
    }

    public function testAssignDoubleToStaticProperty(): void
    {
        Issue2409::setDouble();
        $this->assertSame(3.14, Issue2409::getVar());
    }

    public function testAssignBoolTrueToStaticProperty(): void
    {
        Issue2409::setBoolTrue();
        $this->assertTrue(Issue2409::getVar());
    }

    public function testAssignBoolFalseToStaticProperty(): void
    {
        Issue2409::setBoolFalse();
        $this->assertFalse(Issue2409::getVar());
    }

    public function testAssignCharToStaticProperty(): void
    {
        Issue2409::setChar();
        $this->assertSame(ord('A'), Issue2409::getVar());
    }

    public function testAssignEmptyArrayToStaticProperty(): void
    {
        Issue2409::setEmptyArray();
        $this->assertSame([], Issue2409::getVar());
    }

    public function testAssignArrayToStaticProperty(): void
    {
        Issue2409::setArray();
        $this->assertSame([1, 2, 3], Issue2409::getVar());
    }

    public function testAssignFromStringVariableToStaticProperty(): void
    {
        Issue2409::setFromStringVariable();
        $this->assertSame('hello', Issue2409::getVar());
    }

    public function testAssignFromIntVariableToStaticProperty(): void
    {
        Issue2409::setFromIntVariable();
        $this->assertSame(99, Issue2409::getVar());
    }

    public function testAssignFromDoubleVariableToStaticProperty(): void
    {
        Issue2409::setFromDoubleVariable();
        $this->assertSame(2.5, Issue2409::getVar());
    }

    public function testAssignFromBoolVariableToStaticProperty(): void
    {
        Issue2409::setFromBoolVariable();
        $this->assertTrue(Issue2409::getVar());
    }
}
