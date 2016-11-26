<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2016 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension\Properties;

class StaticPublicPropertiesTest extends \PHPUnit_Framework_TestCase
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
}
