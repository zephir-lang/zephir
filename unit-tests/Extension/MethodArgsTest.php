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
use Test\MethodArgs;

class MethodArgsTest extends TestCase
{
    public function testCallable()
    {
        $t = new MethodArgs();
        $callback = function () {
        };

        $t->setCallable($callback);
        $this->assertSame($callback, $t->a);

        $t->setCallableStrict($callback);
        $this->assertSame($callback, $t->a);

        $this->expectException('\Exception');

        $t->setCallableStrict(true);
    }

    public function testObject()
    {
        $t = new MethodArgs();
        $obj = new \stdClass();

        $t->setObject($obj);
        $this->assertSame($obj, $t->a);

        $t->setObjectStrict($obj);
        $this->assertSame($obj, $t->a);

        $this->expectException('\Exception');

        $t->setObjectStrict(true);
    }

    public function testResource()
    {
        $t = new MethodArgs();

        $t->setResourceStrict(STDIN);
        $this->assertSame(STDIN, $t->a);

        $this->expectException('\Exception');

        $t->setResourceStrict(true);
    }

    public function testMethodOptionalValueWithDefaultStaticConstantAccess()
    {
        $t = new MethodArgs();
        $this->assertSame('test', $t->methodOptionalValueWithDefaultStaticConstantAccess('test'));

        $this->assertSame(MethodArgs::GET, $t->methodOptionalValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::GET, $t->methodOptionalStringValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_DOUBLE, $t->methodOptionalDoubleValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_BOOL, $t->methodOptionalBoolValueWithDefaultStaticConstantAccess());
        $this->assertSame(MethodArgs::MY_INT, $t->methodOptionalIntValueWithDefaultStaticConstantAccess());
    }
}
