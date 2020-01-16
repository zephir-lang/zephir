<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;

class StaticPublicPropertiesTest extends TestCase
{
    public function testAssertations()
    {
        $this->assertNull(\Test\Properties\StaticPublicProperties::$someNull);
        $this->assertNull(\Test\Properties\StaticPublicProperties::$someNullInitial);
        $this->assertFalse(\Test\Properties\StaticPublicProperties::$someFalse);
        $this->assertTrue(\Test\Properties\StaticPublicProperties::$someTrue);
        $this->assertSame(\Test\Properties\StaticPublicProperties::$someInteger, 10);
        $this->assertSame(\Test\Properties\StaticPublicProperties::$someDouble, 10.25);
        $this->assertSame(\Test\Properties\StaticPublicProperties::$someString, 'test');
    }

    public function testIssues1904()
    {
        $value = &\Test\Properties\StaticPublicProperties::$someString;
        $value = 'test1';
        $this->assertSame(\Test\Properties\StaticPublicProperties::$someString, $value);
        \Test\Properties\StaticPublicProperties::$someString = 'test2';
        $this->assertSame(\Test\Properties\StaticPublicProperties::$someString, $value);
        // Disabled due to:
        // https://github.com/phalcon/zephir/issues/1941#issuecomment-538654340
        // \Test\Properties\StaticPublicProperties::setSomeString('test3');
        // $this->assertSame(\Test\Properties\StaticPublicProperties::$someString, $value);
    }

    public function testIssues2020()
    {
        \Test\Properties\StaticPublicProperties::testAddAndSub();
        $this->assertEquals(1, \Test\Properties\StaticPublicProperties::$someAdd);
        $this->assertEquals(-1, \Test\Properties\StaticPublicProperties::$someSub);

        // PHP Notice:  A non well formed numeric value encountered
        //\Test\Properties\StaticPublicProperties::testAddAndSub2();
        //$this->assertEquals(2, \Test\Properties\StaticPublicProperties::$someAdd);
        //$this->assertEquals(-2, \Test\Properties\StaticPublicProperties::$someSub);

        \Test\Properties\StaticPublicProperties::testAddAndSub3();
        $this->assertEquals(2, \Test\Properties\StaticPublicProperties::$someAdd);
        $this->assertEquals(-2, \Test\Properties\StaticPublicProperties::$someSub);

        \Test\Properties\StaticPublicProperties::testAddAndSub4();
        $this->assertEquals(3, \Test\Properties\StaticPublicProperties::$someAdd);
        $this->assertEquals(-3, \Test\Properties\StaticPublicProperties::$someSub);

        \Test\Properties\StaticPublicProperties::testAddAndSub4();
        $this->assertEquals(4, \Test\Properties\StaticPublicProperties::$someAdd);
        $this->assertEquals(-4, \Test\Properties\StaticPublicProperties::$someSub);
    }
}
