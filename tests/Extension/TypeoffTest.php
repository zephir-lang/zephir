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
use Stub\Typeoff;

final class TypeoffTest extends TestCase
{
    private Typeoff $test;

    protected function setUp(): void
    {
        $this->test = new Typeoff();
    }

    public function testNativeString(): void
    {
        $this->assertFalse($this->test->testNativeStringFalse());
        $this->assertTrue($this->test->testNativeStringTrue());
    }

    public function testNativeInt(): void
    {
        $this->assertFalse($this->test->testNativeIntFalse());
        $this->assertTrue($this->test->testNativeIntTrue());
    }

    public function testNativeDouble(): void
    {
        $this->assertTrue($this->test->testNativeDoubleTrue());
    }

    public function testNativeBool(): void
    {
        $this->assertFalse($this->test->testNativeBoolFalse());
        $this->assertTrue($this->test->testNativeBoolTrue());
        $this->assertFalse($this->test->testNotBoolTrue());
    }

    public function testArray(): void
    {
        $this->assertFalse($this->test->testArrayFalse());
        $this->assertTrue($this->test->testArrayTrue());
    }

    public function testClassProperty(): void
    {
        $this->assertTrue($this->test->testClassPropertyAccess());
    }

    public function testCallable(): void
    {
        $callback = function () {
        };
        $this->assertTrue($this->test->testCallableTypeOf($callback));
        $this->assertFalse($this->test->testCallableTypeOf(true));
    }

    public function testUnknown(): void
    {
        $this->assertSame('string', $this->test->testUnknownTypeOf('test'));
        $this->assertSame('integer', $this->test->testUnknownTypeOf(5));
        $this->assertSame('boolean', $this->test->testUnknownTypeOf(true));
        $this->assertSame('double', $this->test->testUnknownTypeOf(5.5));
        $this->assertSame('array', $this->test->testUnknownTypeOf([]));
        $this->assertSame('object', $this->test->testUnknownTypeOf(new \stdClass()));
        $this->assertSame('NULL', $this->test->testUnknownTypeOf(null));
    }
}
