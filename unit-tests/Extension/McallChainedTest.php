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

class McallChainedTest extends \PHPUnit_Framework_TestCase
{
    public function testChained1()
    {
        $t = new \Test\McallChained();
        $this->assertTrue($t->testChained1() === $t);
    }

    public function testChained2()
    {
        $t = new \Test\McallChained();
        $this->assertTrue($t->testChained2() === $t);
    }

    public function testChained3()
    {
        $t = new \Test\McallChained();
        $this->assertTrue($t->testChained3() === $t);
    }
}
