<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use Zephir\Support\TestCase;

class DynamicPropTest extends TestCase
{
    public function testSetProperty()
    {
        $class = new \Test\Oo\DynamicProp();
        $this->assertSame(null, $class->test);

        $class->setPropertyInt('test');
        $this->assertSame(10, $class->test);

        $class->setPropertyBoolTrue('test');
        $this->assertSame(true, $class->test);

        $class->setPropertyBoolFalse('test');
        $this->assertSame(false, $class->test);

        $class->setPropertyString('test');
        $this->assertSame('string', $class->test);

        $class->setPropertyChar('test');
        $this->assertSame(ord('A'), $class->test);

        $class->setPropertyUChar('test');
        $this->assertSame(ord('B') - 1, $class->test);

        $class->setPropertyNull('test');
        $this->assertSame(null, $class->test);

        $class->setPropertyVariableInt('test');
        $this->assertSame(10, $class->test);

        $class->setPropertyVariableString('test');
        $this->assertSame('test', $class->test);

        $class->setPropertyVariableBoolTrue('test');
        $this->assertSame(true, $class->test);

        $class->setPropertyVariableBoolFalse('test');
        $this->assertSame(false, $class->test);

        $class->setPropertyInt('property');
        $this->assertSame(10, $class->property);

        $class->setExistingStringProperty();
        $this->assertSame('works', $class->test);

        $class->setExistingStringPropertyString('test');
        $this->assertSame('test', $class->eproperty);

        $class->setNonExistingStringProperty();
        $this->assertSame('works', $class->property);
    }
}
