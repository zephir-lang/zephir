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
use Stub\MethodArgs;

class MethodArgsTest extends TestCase
{
    private $test;

    protected function setUp(): void
    {
        $this->test = new MethodArgs();
    }

    public function testCallable()
    {
        $callback = function () {
        };

        $this->test->setCallable($callback);
        $this->assertSame($callback, $this->test->a);

        $this->test->setCallableStrict($callback);
        $this->assertSame($callback, $this->test->a);

        $this->expectException(\InvalidArgumentException::class);

        $this->test->setCallableStrict(true);
    }

    public function testObject()
    {
        $obj = new \stdClass();

        $this->test->setObject($obj);
        $this->assertSame($obj, $this->test->a);

        $this->test->setObjectStrict($obj);
        $this->assertSame($obj, $this->test->a);

        $this->expectException(\Throwable::class);

        $this->test->setObjectStrict(true);
    }

    public function testResource()
    {
        $this->test->setResourceStrict(STDIN);
        $this->assertSame(STDIN, $this->test->a);

        $this->expectException(\TypeError::class);

        $this->test->setResourceStrict(true);
    }

    public function testMethodOptionalValueWithDefaultStaticConstantAccess()
    {
        $this->assertSame('test', $this->test->methodOptionalValueWithDefaultStaticConstantAccess('test'));

        $this->assertSame(MethodArgs::GET, $this->test->methodOptionalValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::GET, $this->test->methodOptionalStringValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_DOUBLE, $this->test->methodOptionalDoubleValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_BOOL, $this->test->methodOptionalBoolValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_INT, $this->test->methodOptionalIntValueWithDefaultStaticConstantAccess());
    }
}
