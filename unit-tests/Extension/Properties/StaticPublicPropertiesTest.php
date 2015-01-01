<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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
        $this->assertTrue(\Test\Properties\StaticPublicProperties::$someNull === null);
        $this->assertTrue(\Test\Properties\StaticPublicProperties::$someNullInitial === null);
        $this->assertTrue(\Test\Properties\StaticPublicProperties::$someFalse === false);
        $this->assertTrue(\Test\Properties\StaticPublicProperties::$someTrue === true);
        $this->assertTrue(\Test\Properties\StaticPublicProperties::$someInteger === 10);
        $this->assertTrue(\Test\Properties\StaticPublicProperties::$someDouble === 10.25);
        $this->assertTrue(\Test\Properties\StaticPublicProperties::$someString === "test");
    }
}
