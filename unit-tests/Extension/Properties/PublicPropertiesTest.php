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

use Test\Properties\PublicProperties;
use Test\Properties\ExtendsPublicProperties;

class PublicPropertiesTest extends \PHPUnit_Framework_TestCase
{
    public function testAssertations()
    {
        $t = new PublicProperties();

        $this->assertNull($t->someNull);
        $this->assertNull($t->someNullInitial);
        $this->assertFalse($t->someFalse);
        $this->assertTrue($t->someTrue);
        $this->assertSame($t->someInteger, 10);
        $this->assertSame($t->someDouble, 10.25);
        $this->assertSame($t->someString, 'test');

        $this->assertInternalType('array', $t->someEmptyArray);
        $this->assertSame(array(1, 2, 3, 4), $t->someArray);
    }

    public function test394Issue()
    {
        $t = new PublicProperties();
        $this->assertTrue($t->test394Issue());
    }

    /**
     * @link https://github.com/phalcon/zephir/issues/537
     */
    public function test537Issue()
    {
        $t = new ExtendsPublicProperties();
        $this->assertSame($t->getSomeGetterSetterArray(), array('holy'));
    }
}
