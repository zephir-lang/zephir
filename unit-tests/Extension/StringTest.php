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

class StringTest extends \PHPUnit_Framework_TestCase
{
    public function testTrim()
    {
        $t = new \Test\Strings();

        $this->assertTrue($t->testTrim(" hello ") == "hello");
        $this->assertTrue($t->testTrim("hello ") == "hello");
        $this->assertTrue($t->testTrim(" hello") == "hello");

        $this->assertTrue($t->testTrim2Params('Hello World', "Hdle") == "o Wor");
    }

    public function testLtrim()
    {
        $t = new \Test\Strings();

        $this->assertTrue($t->testLtrim(" hello ") == "hello ");
        $this->assertTrue($t->testLtrim("hello ") == "hello ");
        $this->assertTrue($t->testLtrim(" hello") == "hello");

        $this->assertTrue($t->testLtrim2Params('Hello World', "Hdle") == "o World");
    }

    public function testRtrim()
    {
        $t = new \Test\Strings();

        $this->assertTrue($t->testRtrim(" hello ") == " hello");
        $this->assertTrue($t->testRtrim("hello ") == "hello");
        $this->assertTrue($t->testRtrim(" hello") == " hello");

        $this->assertTrue($t->testRtrim2Params('Hello World', "Hdle") == "Hello Wor");
    }

    public function testStrpos()
    {
        $t = new \Test\Strings();

        $this->assertTrue($t->testStrpos("abcdef abcdef", "a") == 0);
        $this->assertTrue($t->testStrposOffset("abcdef abcdef", "a", 1) == 7);
    }

    public function testImplode()
    {
        $t = new \Test\Strings();

        $pieces = array("a", "b", "c");
        assert($t->testImplode(",", $pieces) == "a,b,c");
    }

    public function testExplode()
    {
        $t = new \Test\Strings();

        $pizza  = "piece1,piece2,piece3,piece4,piece5,piece6";
        $ar1 = $t->testExplode(",", $pizza);
        $ar2 = $t->testExplodeStr($pizza);

        $this->assertTrue($ar1[0] == "piece1");
        $this->assertTrue($ar1[2] == "piece3");

        $this->assertTrue($ar2[0] == "piece1");
        $this->assertTrue($ar2[2] == "piece3");

        $ar3 = $t->testExplodeLimit($pizza, 3);
        $this->assertTrue(count($ar3) == 3);
        $this->assertTrue($ar3[2] == "piece3,piece4,piece5,piece6");
    }

    public function testSubstr()
    {
        $t = new \Test\Strings();

        $this->assertTrue($t->testSubstr('abcdef', 1, 3) == "bcd");
        $this->assertTrue($t->testSubstr('abcdef', 0, 4) == "abcd");
        $this->assertTrue($t->testSubstr('abcdef', 0, 8) == "abcdef");
        $this->assertTrue($t->testSubstr('abcdef', -1, 1) == "f");
        $this->assertTrue($t->testSubstr('abcdef', -3, -1) == "de");
        $this->assertTrue($t->testSubstr('abcdef', 2, -1) == "cde");

        $this->assertTrue($t->testSubstr2('abcdef', -1) == "f");
        $this->assertTrue($t->testSubstr2('abcdef', -2) == "ef");
        $this->assertTrue($t->testSubstr2('abcdef', 2) == "cdef");

        $this->assertTrue($t->testSubstr3('abcdef') == "f");
        $this->assertTrue($t->testSubstr4('abcdef') == "abcde");
    }

    public function testAddslashes()
    {
        $t = new \Test\Strings();

        $this->assertTrue($t->testAddslashes("How's everybody") == addslashes("How's everybody"));
        $this->assertTrue($t->testAddslashes('Are you "JOHN"?') == addslashes('Are you "JOHN"?'));
        $this->assertTrue($t->testAddslashes("hello\0world") == addslashes("hello\0world"));
    }

    public function testStripslashes()
    {
        $t = new \Test\Strings();

        $this->assertTrue($t->testStripslashes(addslashes("How's everybody")) == "How's everybody");
        $this->assertTrue($t->testStripslashes(addslashes('Are you "JOHN"?')) == 'Are you "JOHN"?');
        $this->assertTrue($t->testStripslashes(addslashes("hello\0world")) == "hello\0world");
    }

    public function testMultilineStrings()
    {
        $hardcodedString = '
            Hello world
        ';

        $t = new \Test\Strings();

        $this->assertEquals($hardcodedString, $t->testHardcodedMultilineString());
        ob_start();
        $t->testEchoMultilineString();
        $this->assertEquals($hardcodedString, ob_get_clean());
        $this->assertEquals(trim($hardcodedString), $t->testTrimMultilineString());

        $escapedString = '\"\}\$hello\$\"\\\'';
        $this->assertEquals($escapedString, $t->testWellEscapedMultilineString());
    }
}
