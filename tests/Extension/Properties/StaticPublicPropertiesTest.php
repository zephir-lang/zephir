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

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Stub\Properties\StaticPublicProperties;

final class StaticPublicPropertiesTest extends TestCase
{
    public function testAssertations(): void
    {
        $this->assertNull(StaticPublicProperties::$someNull);
        $this->assertNull(StaticPublicProperties::$someNullInitial);
        $this->assertFalse(StaticPublicProperties::$someFalse);
        $this->assertTrue(StaticPublicProperties::$someTrue);
        $this->assertSame(StaticPublicProperties::$someInteger, 10);
        $this->assertSame(StaticPublicProperties::$someDouble, 10.25);
        $this->assertSame(StaticPublicProperties::$someString, 'test');
    }

    public function testIssues1904(): void
    {
        $value = &StaticPublicProperties::$someString;
        $value = 'test1';
        $this->assertSame(StaticPublicProperties::$someString, $value);
        StaticPublicProperties::$someString = 'test2';
        $this->assertSame(StaticPublicProperties::$someString, $value);
        // Disabled due to:
        // https://github.com/zephir-lang/zephir/issues/1941#issuecomment-538654340
        StaticPublicProperties::setSomeString('test3');
        $this->assertSame(StaticPublicProperties::$someString, $value);
    }

    public function testIssues2020(): void
    {
        StaticPublicProperties::testAddAndSub();
        $this->assertEquals(1, StaticPublicProperties::$someAdd);
        $this->assertEquals(-1, StaticPublicProperties::$someSub);

        // PHP Notice:  A non well formed numeric value encountered
        // \Stub\Properties\StaticPublicProperties::testAddAndSub2();
        // $this->assertEquals(2, \Stub\Properties\StaticPublicProperties::$someAdd);
        // $this->assertEquals(-2, \Stub\Properties\StaticPublicProperties::$someSub);

        StaticPublicProperties::testAddAndSub3();
        $this->assertEquals(2, StaticPublicProperties::$someAdd);
        $this->assertEquals(-2, StaticPublicProperties::$someSub);

        StaticPublicProperties::testAddAndSub4();
        $this->assertEquals(3, StaticPublicProperties::$someAdd);
        $this->assertEquals(-3, StaticPublicProperties::$someSub);

        StaticPublicProperties::testAddAndSub4();
        $this->assertEquals(4, StaticPublicProperties::$someAdd);
        $this->assertEquals(-4, StaticPublicProperties::$someSub);
    }
}
