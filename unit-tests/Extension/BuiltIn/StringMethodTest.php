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

namespace Extension\BuiltIn;


class StringMethodTest extends \PHPUnit_Framework_TestCase
{
    public function testLength()
    {
        $t = new \Test\BuiltIn\StringMethods();

        $this->assertEquals(5, $t->getLength1());
        $this->assertEquals(5, $t->getLength2());
        $this->assertEquals(10, $t->getLength3());
        $this->assertEquals(3, $t->getLength4('foo'));
        $this->assertEquals(8, $t->getLength5('bar'));
    }

    public function testIndex()
    {
        $t = new \Test\BuiltIn\StringMethods();

        $this->assertEquals(0, $t->getIndex('test', 't'));
        $this->assertEquals(3, $t->getIndexWithPosition('test', 't', 1));
    }

    public function testTrim()
    {
        $t = new \Test\BuiltIn\StringMethods();

        $this->assertEquals('hello', $t->getTrimmed());
        $this->assertEquals('foo bar', $t->getTrimmed1(' foo bar  '));
        $this->assertEquals('foo bar  ', $t->getLeftTrimmed(' foo bar  '));
        $this->assertEquals(' foo bar', $t->getRightTrimmed(' foo bar  '));
    }

    public function testChangeCase()
    {
        $t = new \Test\BuiltIn\StringMethods();

        $this->assertEquals('FOO', $t->getUpper('foo'));
        $this->assertEquals('foo', $t->getLower('FOO'));

        $this->assertEquals('Foo', $t->getUpperFirst('foo'));
        $this->assertEquals('fOO', $t->getLowerFirst('FOO'));
    }

    public function testFormat()
    {
        $t = new \Test\BuiltIn\StringMethods();

        $this->assertEquals('hello zephir!', $t->getFormatted('zephir'));
    }
}
 