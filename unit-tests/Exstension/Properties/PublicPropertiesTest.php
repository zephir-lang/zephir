<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

class PublicPropertiesTest extends \PHPUnit_Framework_TestCase
{
    public function testAssertations()
    {
        $t = new \Test\Properties\PublicProperties();

        $this->assertTrue($t->someNull === null);
        $this->assertTrue($t->someNullInitial === null);
        $this->assertTrue($t->someFalse === false);
        $this->assertTrue($t->someTrue === true);
        $this->assertTrue($t->someInteger === 10);
        $this->assertTrue($t->someDouble === 10.25);
        $this->assertTrue($t->someString === "test");
    }
}
