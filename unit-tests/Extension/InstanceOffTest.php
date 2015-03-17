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

namespace Extension;

use Test\InstanceOff;

class InstanceOffTest extends \PHPUnit_Framework_TestCase
{
    public function testInstanceOf1()
    {
        $t = new InstanceOff();
        $this->assertTrue($t->testInstanceOf1() === true);
    }

    public function testInstanceOf2()
    {
        $t = new InstanceOff();
        $this->assertTrue($t->testInstanceOf2() === true);
    }

    public function testInstanceOf3()
    {
        $t = new InstanceOff();
        $this->assertTrue($t->testInstanceOf3() === false);
    }

    public function testInstanceOf4()
    {
        $t = new InstanceOff();

        $this->assertTrue($t->testInstanceOf4(new \ArrayIterator(array())));
        $this->assertTrue($t->testInstanceOf4(new \ArrayObject()));

        $this->assertFalse($t->testInstanceOf4(1));
        $this->assertFalse($t->testInstanceOf4(1.25));
        $this->assertFalse($t->testInstanceOf4(true));
        $this->assertFalse($t->testInstanceOf4(false));
        $this->assertFalse($t->testInstanceOf4("test"));
        $this->assertFalse($t->testInstanceOf4(array()));
        $this->assertFalse($t->testInstanceOf4(new \StdClass));
    }

    /**
     * @link https://github.com/phalcon/zephir/issues/776
     */
    public function testInstanceOf5()
    {
        $t = new InstanceOff();

        $this->assertTrue($t->testInstanceOf5($t));

        /**
         * Cannot test instance of for not objects
         * @link https://github.com/phalcon/zephir/issues/822
         */

        $this->assertFalse($t->testInstanceOf5(new \StdClass));
    }
}
