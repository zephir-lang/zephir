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
use Stub\Oo\DynamicProp;

final class DynamicPropTest extends TestCase
{
    public function testSetProperty(): void
    {
        if (version_compare(PHP_VERSION, '8.2.0', '>=')) {
            $this->markTestSkipped(' Creation of dynamic property is deprecated');
        }

        $class = new DynamicProp();
        $this->assertNull($class->test);

        $class->setPropertyInt('test');
        $this->assertSame(10, $class->test);

        $class->setPropertyBoolTrue('test');
        $this->assertTrue($class->test);

        $class->setPropertyBoolFalse('test');
        $this->assertFalse($class->test);

        $class->setPropertyString('test');
        $this->assertSame('string', $class->test);

        $class->setPropertyChar('test');
        $this->assertSame(\ord('A'), $class->test);

        $class->setPropertyUChar('test');
        $this->assertSame(\ord('B') - 1, $class->test);

        $class->setPropertyNull('test');
        $this->assertNull($class->test);

        $class->setPropertyVariableInt('test');
        $this->assertSame(10, $class->test);

        $class->setPropertyVariableString('test');
        $this->assertSame('test', $class->test);

        $class->setPropertyVariableBoolTrue('test');
        $this->assertTrue($class->test);

        $class->setPropertyVariableBoolFalse('test');
        $this->assertFalse($class->test);

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
