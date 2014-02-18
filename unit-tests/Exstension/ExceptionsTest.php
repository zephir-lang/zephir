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

namespace Exstension;

class ExceptionsTest
    extends \PHPUnit_Framework_TestCase
{
    /**
     * @expectedException \Exception
     *
     */
    public function testException1()
    {
        $this->setExpectedException('\Test\Exception', 'hello1');

        $t = new \Test\Exceptions();
        $t->testException1();
    }

    public function testException2()
    {
        $this->setExpectedException('\Test\Exception', 'hello2');

        $t = new \Test\Exceptions();
        $t->testException2();
    }

    public function testException3()
    {
        $this->setExpectedException('\Test\Exception', 'hello3');

        $t = new \Test\Exceptions();
        $t->testException3();
    }

    public function testException4()
    {
        $this->setExpectedException('\Test\Exception', 'hello4');

        $t = new \Test\Exceptions();
        $t->testException4();
    }
} 