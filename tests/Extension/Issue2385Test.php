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
use Stub\Issue2385;

/**
 * Regression coverage for isset() semantics on object properties and array
 * offsets — Zephir's isset() must match PHP's isset(), i.e. it must return
 * false when the value is null (not only when the key/property is missing).
 *
 * @see https://github.com/zephir-lang/zephir/issues/2385
 */
final class Issue2385Test extends TestCase
{
    private Issue2385 $sut;

    protected function setUp(): void
    {
        $this->sut = new Issue2385();
    }

    public function testDeclaredOnlyPropertyIsNotSet(): void
    {
        // Property declared on the class but never assigned. PHP isset() = false.
        $this->assertFalse($this->sut->checkDeclaredOnly());
    }

    public function testPropertyAssignedNullIsNotSet(): void
    {
        $this->assertFalse($this->sut->checkAssignedNull());
    }

    public function testPropertyAssignedValueIsSet(): void
    {
        $this->assertTrue($this->sut->checkAssignedValue());
    }

    public function testDynamicPropertyNameAssignedNullIsNotSet(): void
    {
        $this->assertFalse($this->sut->checkDynamicKeyNull('assignedNull'));
    }

    public function testDynamicPropertyNameAssignedValueIsSet(): void
    {
        $this->assertTrue($this->sut->checkDynamicKeyNull('assignedValue'));
    }

    public function testArrayStringKeyHoldingNullIsNotSet(): void
    {
        $this->assertFalse($this->sut->checkArrayKeyNull());
    }

    public function testArrayStringKeyHoldingValueIsSet(): void
    {
        $this->assertTrue($this->sut->checkArrayKeyValue());
    }

    public function testArrayStringKeyMissingIsNotSet(): void
    {
        $this->assertFalse($this->sut->checkArrayKeyMissing());
    }

    public function testArrayLongKeyHoldingNullIsNotSet(): void
    {
        $this->assertFalse($this->sut->checkArrayLongKeyNull());
    }

    public function testArrayLongKeyHoldingValueIsSet(): void
    {
        $this->assertTrue($this->sut->checkArrayLongKeyValue());
    }

    public function testArrayVariableKeyHoldingNullIsNotSet(): void
    {
        $this->assertFalse($this->sut->checkArrayVarKeyNull('a'));
    }

    public function testArrayVariableKeyHoldingValueIsSet(): void
    {
        $this->assertTrue($this->sut->checkArrayVarKeyValue('a'));
    }
}
