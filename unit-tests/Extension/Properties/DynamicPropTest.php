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

use Test\Oo\DynamicProp;

class DynamicPropTest extends \PHPUnit_Framework_TestCase
{
    public function testAssertations()
    {
        $t = new DynamicProp();
        $testProperyName = 'test';

        $t->setPropertyInt($testProperyName);
        $this->assertEquals(10, $t->getProperty($testProperyName));
        $t->setPropertyBool($testProperyName);
        $this->assertEquals(true, $t->getProperty($testProperyName));
        $t->setPropertyString($testProperyName);
        $this->assertEquals(true, $t->getProperty($testProperyName));
        $t->setPropertyChar($testProperyName);
        $this->assertEquals('A', $t->getProperty($testProperyName));
        $t->setPropertyUChar($testProperyName);
        $this->assertEquals('B', $t->getProperty($testProperyName));
    }
}
