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

use Test\Exceptions;

class ExceptionsTest extends \PHPUnit_Framework_TestCase
{
    /**
     * @expectedException Test\Exception
     * @expectedExceptionMessage hello1
     */
    public function testException1()
    {
        $t = new Exceptions();
        $t->testException1();
    }

    /**
     * @expectedException Test\Exception
     * @expectedExceptionMessage hello2
     */
    public function testException2()
    {
        $t = new Exceptions();
        $t->testException2();
    }

    /**
     * @expectedException Test\Exception
     * @expectedExceptionMessage hello3
     */
    public function testException3()
    {
        $t = new Exceptions();
        $t->testException3();
    }

    /**
     * @expectedException Test\Exception
     * @expectedExceptionMessage hello4
     */
    public function testException4()
    {
        $t = new Exceptions();
        $t->testException4();
    }

    /**
     * @expectedException Test\Exception
     * @expectedExceptionMessage hello5
     */
    public function testException5()
    {
        $t = new Exceptions();
        $t->testException5();
    }

    /**
     * @expectedException Exception
     * @expectedExceptionMessage Test
     */
    public function testExceptionString()
    {
        $t = new Exceptions();
        $t->testExceptionLiteral('string');
    }

    /**
     * @expectedException Exception
     * @expectedExceptionMessage t
     */
    public function testExceptionChar()
    {
        $t = new Exceptions();
        $t->testExceptionLiteral('char');
    }

    /**
     * @expectedException Exception
     * @expectedExceptionMessage 123
     */
    public function testExceptionInt()
    {
        $t = new Exceptions();
        $t->testExceptionLiteral('int');
    }

    /**
     * @expectedException Exception
     * @expectedExceptionMessage 123.123
     */
    public function testExceptionDouble()
    {
        $t = new Exceptions();
        $t->testExceptionLiteral('double');
    }

    /**
     * @expectedException Exception
     * @expectedExceptionMessage Hello, Zephir
     */
    public function testExceptionSprintf()
    {
        $t = new Exceptions();
        $t->testExceptionSprintf('Zephir');
    }

    /**
     * @expectedException Exception
     * @expectedExceptionMessage Framework Phalcon written using Zephir
     */
    public function testExceptionConcat()
    {
        $t = new Exceptions();
        $t->testExceptionConcat('Phalcon', 'Zephir');
    }

    public function testExceptionRethrow()
    {
        $t = new Exceptions();
        try {
            $t->testExceptionRethrow();
            $this->assertFalse();
        } catch (\Exception $e) {
            $this->assertEquals(8, $e->getLine());
        }
    }
}
