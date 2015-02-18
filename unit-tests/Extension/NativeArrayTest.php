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

use Test\NativeArray;

class NativeArrayTest extends \PHPUnit_Framework_TestCase
{
    public function testArray()
    {
        $t = new NativeArray();
        $this->assertTrue($t->testArray1() == array());
        $this->assertTrue($t->testArray2() == array(1, 2, 3));
        $this->assertTrue($t->testArray3() == array(1.1, 2.2, 3.3));
        $this->assertTrue($t->testArray4() == array(false, true, false));
        $this->assertTrue($t->testArray5() == array(null, null, null));
        $this->assertTrue($t->testArray6() == array("x", "y", "z"));
        $this->assertTrue($t->testArray7() == array(1, 2, 3));
        $this->assertTrue($t->testArray8() == array(1.0, 2.0, 3.0));
        $this->assertTrue($t->testArray9() == array(true, false, true));
        $this->assertTrue($t->testArray10() == array("hello1",  "hello2",  "hello3"));
        $this->assertTrue($t->testArray11() == array(array(1, 2, 3), array(4, 5, 6)));
        $this->assertTrue($t->testArray12() == array(array(array(1, 2, 3))));
        $this->assertTrue($t->testArray13() == array(1 => "hello1", 2 => "hello2", 3 => "hello3"));
        $this->assertTrue($t->testArray14() == array("hello1" => 1, "hello2" => 2, "hello3" => 3));
        $this->assertTrue($t->testArray15() == array("hello1" => true, "hello2" => false, "hello3" => true));
        $this->assertTrue($t->testArray16() == array("hello1" => 1.0, "hello2" => 2.0, "hello3" => 3.0));
        $this->assertTrue($t->testArray17() == array("hello1" => null, "hello2" => null, "hello3" => null));
        $this->assertTrue($t->testArray18() == array("hello1" => "a", "hello2" => "b", "hello3" => "c"));
        $this->assertTrue($t->testArray19() == array(0 => true, 1 => false, 2 => true));
        $this->assertTrue($t->testArray20() == array(0 => 1.0, 1 => 2.0, 2 => 3.0));
        $this->assertTrue($t->testArray21() == array(0 => null, 1 => null, 2 => null));
        $this->assertTrue($t->testArray22() == array(0 => 4, 1 => 5, 2 => 6));
        $this->assertTrue($t->testArray23() == array(0 => 0, 1 => 0, 2 => 0));
        $this->assertTrue($t->testArray24() == array(0 => 0.0, 1 => 0.0, 2 => 0.0));
    }

    public function testArrayAccess()
    {
        $t = new NativeArray();
        $this->assertTrue($t->testArrayAccess1() == 1);
        $this->assertTrue($t->testArrayAccess2() == 1);
        $this->assertTrue($t->testArrayAccess3() == 1);
        $this->assertTrue($t->testArrayAccess4() == 1);
        $this->assertTrue($t->testArrayAccess5() == 1);
    }

    public function testArrayMultipleAccess()
    {
        $t = new NativeArray();
        $this->assertTrue($t->testArrayMultipleAccess1() == "a");
        $this->assertTrue($t->testArrayMultipleAccess2() == "b");
        $this->assertTrue($t->testArrayMultipleAccess3() == "b");
        $this->assertTrue($t->testArrayMultipleAccess4() == "b");
        $this->assertTrue($t->testArrayMultipleAccess5() == 0);
    }

    public function testArrayUpdate()
    {
        $t = new NativeArray();
        $this->assertTrue($t->testArrayUpdate1() == array(4, 2, 3));
        $this->assertTrue($t->testArrayUpdate2() == array(4, 2, 3));
        $this->assertTrue($t->testArrayUpdate3() == array(4, 5, 3));
        $this->assertTrue($t->testArrayUpdate4() == array("a" => 4, "b" => 2, "c" => 3));
    }

    public function testMultipleArrayUpdate()
    {
        $t = new NativeArray();
        $this->assertTrue($t->testMultipleArrayUpdate1() == array('x' => array('y' => null)));
        $this->assertTrue($t->testMultipleArrayUpdate2() == array('x' => array('y' => array('z' => null))));
        $this->assertTrue($t->testMultipleArrayUpdate3() == array(0 => array(1 => null)));
        $this->assertTrue($t->testMultipleArrayUpdate4() == array(0 => array(1 => array(2 => null))));
        $this->assertTrue($t->testMultipleArrayUpdate5() == array('a' => array(1 => array('c' => null))));
        $this->assertTrue($t->testMultipleArrayUpdate6() == array(0 => array('b' => array(2 => null))));
        $this->assertTrue($t->testMultipleArrayUpdate7() == array('a' => array(1 => array('c' => true)), 0 => array('b' => array(2 => false))));
        $this->assertTrue($t->testMultipleArrayUpdate8() == array('a' => array(0 => null, 1 => true, 2 => false)));
        $this->assertTrue($t->testMultipleArrayUpdate9() == array('a' => array(0 => null, 1 => false)));
        $this->assertTrue($t->testMultipleArrayUpdate10() == array('a' => array(0 => null, 1 => array('c' => false))));
        $this->assertTrue($t->testMultipleArrayUpdate11() == array('y' => array('x' => null)));
        $this->assertTrue($t->testMultipleArrayUpdate12() == array('y' => array('x' => array('z' => null))));
    }

    public function testArrayKeys()
    {
        $t = new NativeArray();
        $this->assertTrue($t->testArrayKeys(array('test1' => 1, 'test2' => 2, 'test3' => 3)) === array('test1', 'test2', 'test3'));
        $this->assertTrue($t->testArrayKeys(array(1, 2, 3, 4, 5, 6)) === array(0, 1, 2, 3, 4, 5));
    }

    public function testImplodeArray()
    {
        $t = new NativeArray();
        $this->assertTrue($t->testImplodeArray(array('test1' => 1, 'test2' => 2, 'test3' => 3)) === 'test1|test2|test3');
    }

    public function testIssue110()
    {
        $t = new NativeArray();
        $this->assertTrue($t->issue110() === 'B|K|M|G|T|KB|MB|GB|TB');
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
        $this->assertEquals($expected, $t->issue743a(array(42 => array())));
        $this->assertEquals($expected, $t->issue743a(array(42 => array("str" => null))));
        $this->assertEquals($expected, $t->issue743a(array(42 => array("str" => 42.7))));
        $this->assertEquals($expected, $t->issue743a(array(42 => array("str" => 42))));
        $this->assertEquals($expected, $t->issue743a(array(42 => array("str" => true))));
        $this->assertEquals($expected, $t->issue743a(array(42 => array("str" => "bad"))));
        $this->assertEquals($expected, $t->issue743a(array(42 => array("str" => array()))));
        $this->assertEquals($expected, $t->issue743a(array(42 => array("str" => array("hey")))));
        $this->assertEquals($expected, $t->issue743a(array(42 => array("str" => new \stdClass()))));

        $expected = array("str" => array(42 => "ok"));
        $this->assertEquals($expected, $t->issue743b(array("str" => array())));
        $this->assertEquals($expected, $t->issue743b(array("str" => array(42 => null))));
        $this->assertEquals($expected, $t->issue743b(array("str" => array(42 => 42.7))));
        $this->assertEquals($expected, $t->issue743b(array("str" => array(42 => 42))));
        $this->assertEquals($expected, $t->issue743b(array("str" => array(42 => true))));
        $this->assertEquals($expected, $t->issue743b(array("str" => array(42 => "bad"))));
        $this->assertEquals($expected, $t->issue743b(array("str" => array(42 => array()))));
        $this->assertEquals($expected, $t->issue743b(array("str" => array(42 => array("hey")))));
        $this->assertEquals($expected, $t->issue743b(array("str" => array(42 => new \stdClass()))));

        $expected = array("str" => array("hey" => "ok"));
        $this->assertEquals($expected, $t->issue743c(array("str" => array())));
        $this->assertEquals($expected, $t->issue743c(array("str" => array("hey" => null))));
        $this->assertEquals($expected, $t->issue743c(array("str" => array("hey" => 42.7))));
        $this->assertEquals($expected, $t->issue743c(array("str" => array("hey" => 42))));
        $this->assertEquals($expected, $t->issue743c(array("str" => array("hey" => true))));
        $this->assertEquals($expected, $t->issue743c(array("str" => array("hey" => "bad"))));
        $this->assertEquals($expected, $t->issue743c(array("str" => array("hey" => array()))));
        $this->assertEquals($expected, $t->issue743c(array("str" => array("hey" => array("hey")))));
        $this->assertEquals($expected, $t->issue743c(array("str" => array("hey" => new \stdClass()))));
    }
    
    public function testIssue709()
    {
        $t = new NativeArray();
        $this->assertTrue($t->Issue709());
    }
}
