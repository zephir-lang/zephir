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

use Test\NativeArray;

class NativeArrayTest extends \PHPUnit_Framework_TestCase
{
    public function testArray()
    {
        $t = new NativeArray();
        $this->assertSame($t->testArray1(), array());
        $this->assertSame($t->testArray2(), array(1, 2, 3));
        $this->assertSame($t->testArray3(), array(1.1, 2.2, 3.3));
        $this->assertSame($t->testArray4(), array(false, true, false));
        $this->assertSame($t->testArray5(), array(null, null, null));
        $this->assertSame($t->testArray6(), array("x", "y", "z"));
        $this->assertSame($t->testArray7(), array(1, 2, 3));
        $this->assertSame($t->testArray8(), array(1.0, 2.0, 3.0));
        $this->assertSame($t->testArray9(), array(true, false, true));
        $this->assertSame($t->testArray10(), array("hello1",  "hello2",  "hello3"));
        $this->assertSame($t->testArray11(), array(array(1, 2, 3), array(4, 5, 6)));
        $this->assertSame($t->testArray12(), array(array(array(1, 2, 3))));
        $this->assertSame($t->testArray13(), array(1 => "hello1", 2 => "hello2", 3 => "hello3"));
        $this->assertSame($t->testArray14(), array("hello1" => 1, "hello2" => 2, "hello3" => 3));
        $this->assertSame($t->testArray15(), array("hello1" => true, "hello2" => false, "hello3" => true));
        $this->assertSame($t->testArray16(), array("hello1" => 1.0, "hello2" => 2.0, "hello3" => 3.0));
        $this->assertSame($t->testArray17(), array("hello1" => null, "hello2" => null, "hello3" => null));
        $this->assertSame($t->testArray18(), array("hello1" => "a", "hello2" => "b", "hello3" => "c"));
        $this->assertSame($t->testArray19(), array(0 => true, 1 => false, 2 => true));
        $this->assertSame($t->testArray20(), array(0 => 1.0, 1 => 2.0, 2 => 3.0));
        $this->assertSame($t->testArray21(), array(0 => null, 1 => null, 2 => null));
        $this->assertSame($t->testArray22(), array(0 => 4, 1 => 5, 2 => 6));
        $this->assertSame($t->testArray23(), array(0 => 0, 1 => 0, 2 => 0));
        $this->assertSame($t->testArray24(), array(0 => 0.0, 1 => 0.0, 2 => 0.0));
    }

    public function testArrayAccess()
    {
        $t = new NativeArray();
        $this->assertSame($t->testArrayAccess1(), 1);
        $this->assertSame($t->testArrayAccess2(), 1);
        $this->assertSame($t->testArrayAccess3(), 1);
        $this->assertSame($t->testArrayAccess4(), 1);
        $this->assertSame($t->testArrayAccess5(), 1);
    }

    public function testArrayMultipleAccess()
    {
        $t = new NativeArray();
        $this->assertSame($t->testArrayMultipleAccess1(), "a");
        $this->assertSame($t->testArrayMultipleAccess2(), "b");
        $this->assertSame($t->testArrayMultipleAccess3(), "b");
        $this->assertSame($t->testArrayMultipleAccess4(), "b");
        $this->assertSame($t->testArrayMultipleAccess5(), 0);
    }

    public function testArrayUpdate()
    {
        $t = new NativeArray();
        $this->assertSame($t->testArrayUpdate1(), array(4, 2, 3));
        $this->assertSame($t->testArrayUpdate2(), array(4, 2, 3));
        $this->assertSame($t->testArrayUpdate3(), array(4, 5, 3));
        $this->assertSame($t->testArrayUpdate4(), array("a" => 4, "b" => 2, "c" => 3));
    }

    public function testMultipleArrayUpdate()
    {
        $t = new NativeArray();
        $this->assertSame($t->testMultipleArrayUpdate1(), array('x' => array('y' => null)));
        $this->assertSame($t->testMultipleArrayUpdate2(), array('x' => array('y' => array('z' => null))));
        $this->assertSame($t->testMultipleArrayUpdate3(), array(0 => array(1 => null)));
        $this->assertSame($t->testMultipleArrayUpdate4(), array(0 => array(1 => array(2 => null))));
        $this->assertSame($t->testMultipleArrayUpdate5(), array('a' => array(1 => array('c' => null))));
        $this->assertSame($t->testMultipleArrayUpdate6(), array(0 => array('b' => array(2 => null))));
        $this->assertSame($t->testMultipleArrayUpdate7(), array('a' => array(1 => array('c' => true)), 0 => array('b' => array(2 => false))));
        $this->assertSame($t->testMultipleArrayUpdate8(), array('a' => array(0 => null, 1 => true, 2 => false)));
        $this->assertSame($t->testMultipleArrayUpdate9(), array('a' => array(0 => null, 1 => false)));
        $this->assertSame($t->testMultipleArrayUpdate10(), array('a' => array(0 => null, 1 => array('c' => false))));
        $this->assertSame($t->testMultipleArrayUpdate11(), array('y' => array('x' => null)));
        $this->assertSame($t->testMultipleArrayUpdate12(), array('y' => array('x' => array('z' => null))));
    }

    public function testArrayKeys()
    {
        $t = new NativeArray();
        $this->assertSame($t->testArrayKeys(array('test1' => 1, 'test2' => 2, 'test3' => 3)), array('test1', 'test2', 'test3'));
        $this->assertSame($t->testArrayKeys(array(1, 2, 3, 4, 5, 6)), array(0, 1, 2, 3, 4, 5));
    }

    public function testImplodeArray()
    {
        $t = new NativeArray();
        $this->assertSame($t->testImplodeArray(array('test1' => 1, 'test2' => 2, 'test3' => 3)), 'test1|test2|test3');
    }

    public function testIssue110()
    {
        $t = new NativeArray();
        $this->assertSame($t->issue110(), 'B|K|M|G|T|KB|MB|GB|TB');
    }

    public function testIssue264()
    {
        $t = new NativeArray();
        $this->assertFalse($t->issue264(array(1, 2, 3)));
    }

    public function testIssue743()
    {
        $t = new NativeArray();

        $expected = array(42 => array("str" => "ok"));
        $this->assertSame($expected, $t->issue743a(array(42 => array())));
        $this->assertSame($expected, $t->issue743a(array(42 => array("str" => null))));
        $this->assertSame($expected, $t->issue743a(array(42 => array("str" => 42.7))));
        $this->assertSame($expected, $t->issue743a(array(42 => array("str" => 42))));
        $this->assertSame($expected, $t->issue743a(array(42 => array("str" => true))));
        $this->assertSame($expected, $t->issue743a(array(42 => array("str" => "bad"))));
        $this->assertSame($expected, $t->issue743a(array(42 => array("str" => array()))));
        $this->assertSame($expected, $t->issue743a(array(42 => array("str" => array("hey")))));
        $this->assertSame($expected, $t->issue743a(array(42 => array("str" => new \stdClass()))));

        $expected = array("str" => array(42 => "ok"));
        $this->assertSame($expected, $t->issue743b(array("str" => array())));
        $this->assertSame($expected, $t->issue743b(array("str" => array(42 => null))));
        $this->assertSame($expected, $t->issue743b(array("str" => array(42 => 42.7))));
        $this->assertSame($expected, $t->issue743b(array("str" => array(42 => 42))));
        $this->assertSame($expected, $t->issue743b(array("str" => array(42 => true))));
        $this->assertSame($expected, $t->issue743b(array("str" => array(42 => "bad"))));
        $this->assertSame($expected, $t->issue743b(array("str" => array(42 => array()))));
        $this->assertSame($expected, $t->issue743b(array("str" => array(42 => array("hey")))));
        $this->assertSame($expected, $t->issue743b(array("str" => array(42 => new \stdClass()))));

        $expected = array("str" => array("hey" => "ok"));
        $this->assertSame($expected, $t->issue743c(array("str" => array())));
        $this->assertSame($expected, $t->issue743c(array("str" => array("hey" => null))));
        $this->assertSame($expected, $t->issue743c(array("str" => array("hey" => 42.7))));
        $this->assertSame($expected, $t->issue743c(array("str" => array("hey" => 42))));
        $this->assertSame($expected, $t->issue743c(array("str" => array("hey" => true))));
        $this->assertSame($expected, $t->issue743c(array("str" => array("hey" => "bad"))));
        $this->assertSame($expected, $t->issue743c(array("str" => array("hey" => array()))));
        $this->assertSame($expected, $t->issue743c(array("str" => array("hey" => array("hey")))));
        $this->assertSame($expected, $t->issue743c(array("str" => array("hey" => new \stdClass()))));
    }

    public function testIssue709()
    {
        $t = new NativeArray();
        $this->assertTrue($t->Issue709());
    }
}
