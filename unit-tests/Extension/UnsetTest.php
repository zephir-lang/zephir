<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

use Test\Unsettest as UnsetTestExt;

class UnsetTest extends \PHPUnit_Framework_TestCase
{
    public function testUnsetFromArray()
    {
        $t = new UnsetTestExt();

        $testVar = array(1, 2, 3, 4, 5, 6);
        $this->assertSame(array(1 => 2, 3, 4, 5, 6), $t->testUnsetFromArray($testVar));
        $this->assertSame(array(1 => 2, 3, 4, 5, 6), $t->testUnsetFromArrayByIndexVar($testVar, 0));

        $this->assertSame(array('foo' => 'bar'), $t->testUnsetTypedArray(array('foo' => 'bar', 'bar' => 'foo'), 'bar'));
    }

    public function xtestUnsetValueFromProperty()
    {
        //@todo Zephir is not generating the correct code to make this work
        $t = new UnsetTestExt();

        $testVar = array(1, 2, 3, 4, 5, 6);
        $this->assertFalse($t->has('testVar'));
        $t->addValueToProperty('testVar', $testVar);
        $this->assertTrue($t->has('testVar'));
        $t->testUnsetValueFromProperty('testVar');
        $this->assertFalse($t->has('testVar'));

        $t->testUnsetProperty();
        $this->assertSame(null, $t->getProperty());
    }

    public function xtestStdClassUnset()
    {
        //@todo Zephir is not generating the correct code to make this work
        $t = new UnsetTestExt();

        $equalsStdClass = new \StdClass;
        $equalsStdClass->property1 = 12345;
        $equalsStdClass->property3 = 12345;

        $this->assertSame($equalsStdClass, $t->testStdClassUnset());
    }
}
