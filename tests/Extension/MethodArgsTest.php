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
use Stub\MethodArgs;

final class MethodArgsTest extends TestCase
{
    private MethodArgs $test;

    protected function setUp(): void
    {
        $this->test = new MethodArgs();
    }

    public function testCallable(): void
    {
        $callback = function (): void {
        };

        $this->test->setCallable($callback);
        $this->assertSame($callback, $this->test->a);

        $this->test->setCallableStrict($callback);
        $this->assertSame($callback, $this->test->a);

        $this->expectException(\InvalidArgumentException::class);

        $this->test->setCallableStrict(true);
    }

    public function testObject(): void
    {
        $obj = new \stdClass();

        $this->test->setObject($obj);
        $this->assertSame($obj, $this->test->a);

        $this->test->setObjectStrict($obj);
        $this->assertSame($obj, $this->test->a);

        $this->expectException(\Throwable::class);

        $this->test->setObjectStrict(true);
    }

    public function testResource(): void
    {
        $this->test->setResourceStrict(STDIN);
        $this->assertSame(STDIN, $this->test->a);

        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->expectException(\TypeError::class);
        } else {
            $this->expectException(\InvalidArgumentException::class);
        }

        $this->test->setResourceStrict(true);
    }

    public function testMethodOptionalValueWithDefaultStaticConstantAccess(): void
    {
        $this->assertSame('test', $this->test->methodOptionalValueWithDefaultStaticConstantAccess('test'));

        $this->assertSame(MethodArgs::GET, $this->test->methodOptionalValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::GET, $this->test->methodOptionalStringValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_DOUBLE, $this->test->methodOptionalDoubleValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_BOOL, $this->test->methodOptionalBoolValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_INT, $this->test->methodOptionalIntValueWithDefaultStaticConstantAccess());
    }
}
